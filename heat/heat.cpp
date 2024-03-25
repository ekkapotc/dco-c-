// +++++++++++++++++++++++++++++++++++++++++
// simple data assimilation with the 
// one-dimensional heat equation 
// +++++++++++++++++++++++++++++++++++++++++

#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include "../dco_a1s_type.hpp"

//plot
#include <unistd.h>

extern dco_a1s_tape_entry * dco_a1s_tape;
 
#define MAX_TAPE_ELEMS 10000000
#define MAX_ITERATIONS 10000

template <class T>
void ts(
        int & nx, 
        double & delta_t, 
        double & c,
        T * temp
       ) 
{ 	
  T * old_temp { new T[nx+1] }; 

  for( auto j{0} ; j<=nx ; j++ ){
  	old_temp[j] = temp[j];
  }

  for( auto j{0} ; j<=nx ; j++ ){
    if(j>0 && j<nx)
    {
      	auto jp1 {j+1};
      	auto jm1 {j-1};
      	temp[j]=old_temp[j]+(nx*nx)*c*delta_t*(old_temp[jp1]-2*old_temp[j]+old_temp[jm1]);
     }
  }
  
  delete [] old_temp;
}

template <class T>
void evolve(int& nx,
        int& from,
        int& to,
        int& stride,
        double & delta_t,
        double & a,
        double & b,
        double & c,
        T * temp)
{
  auto i{0};
  auto nt{0};
  auto lto{0};
  	
  nt=to-from;
  
  if(nt>stride){
    	lto=from+nt/2;
    	evolve(nx,from,lto,stride,delta_t,a,b,c,temp);
    	lto=from+nt/2+1;
    	evolve(nx,lto,to,stride,delta_t,a,b,c,temp);
   }else{
     	// time integration
     	i=from;

     	while(i<=to){
       		ts(nx,delta_t,c,temp);
       		i=i+1;
     	}
   }
}

template <class T>
T objFunc
	(
       	int nx, 
       	int nt, 
       	int stride, 
       	double delta_t, 
       	double a,
       	double b,
       	double c,
       	double * temp_obs,
       	T * temp
       ) 
{
  auto zero {0};
  T cost;

  evolve(nx,zero,nt,stride,delta_t,a,b,c,temp);
  	
  auto j{1};

  while(j<nx){
    cost +=  (temp[j]-temp_obs[j])*(temp[j]-temp_obs[j]);
    j = j+1;
  }

  return cost;

}

double get_norm( double * v , int ndim  ){
  double res {0.00};
  for( auto i=0 ; i<ndim ; i++ ) res += v[i]*v[i];
  return std::sqrt(res);
}

int main(int argc, char* argv[])
{
  auto nx { atoi(argv[1]) }; 
  auto nt { atoi(argv[2]) };
  auto constexpr  max_tape_elems {MAX_TAPE_ELEMS};
  	
  //Set up epsilon
  double epsilon {1.0e-5};
 
  //Declare gradient norm and gradient vector
  double grad_norm;
  double * grad {new double[nx+1]};
  
  //Set up observations
  double * temp_obs { new double[nx+1] }; 
  for(auto i{0};i<=nx;i++){
    temp_obs[i] = 2.0-(i/100.0);
  }

  //Create a DCO Tape
  dco_a1s_create_tape(max_tape_elems);

  //Declare inputs
  dco_a1s_type * temp { new dco_a1s_type[nx+1] };
  double * temp_old{ new double[nx+1] };
  
  //Set up initial condition
  temp[0] = 2.0;
  temp_old[0] = temp[0].v;
     
  for( auto i{1} ; i<=nx ; i++ ){
    temp[i] = 0.0;
    temp_old[i] = temp[i].v;
  }

  auto a {2.0};
  auto b {0.0};
  auto c { 0.001 };
  auto stride{10}; 
  auto delta_t {1.0/nt};
   
  //Set up iteration counter
  int iters{0};
  constexpr int max_iterations{MAX_ITERATIONS};

  //Create a pipe for GNUPlot
  FILE * pipe = popen("gnuplot --persist","w");

  do{
      //Register the independent variables
      for( auto i=0 ; i<=nx ; i++ ){
        dco_a1s_register_independent(temp[i]);
      }

      dco_a1s_type cost = objFunc<dco_a1s_type>(nx,nt,stride,delta_t,a,b,c,temp_obs,temp);
     
      //Register the dependent variable
      dco_a1s_register_dependent(cost);
 
      dco_a1s_tape[cost.va].a = 1.0;
	
      //Perform backward pass
      dco_a1s_interpret_tape();

      for( auto i{0} ; i<=nx ; i++ ){
        grad[i] = dco_a1s_tape[dco_a1s_indep_va[i]].a;
      }

      //Compute the norm of grad
      grad_norm = get_norm(grad,nx);

      double loss {cost.v};
      double loss_new {loss};

      //Intialize learning rate at 0.10
      double learning_rate = 0.10;

      //Perform line search
      while( loss_new>=loss ){

        for( auto i{0} ; i<=nx ; i++){
            temp[i].v = temp_old[i] - learning_rate*grad[i];
            temp_old[i] = temp[i].v;
         }

	       loss_new = objFunc<double>(nx,nt,stride,delta_t,a,b,c,temp_obs,temp_old);
    
         //Update learning rate 
         learning_rate *= 0.50;
      }

      std::cout << "Iteration : " << iters+1 << " : " << "New Loss = " << loss_new << " , Gradient Norm = " << grad_norm << std::endl;
      
      /*Plotting code Starts here.*/
      fprintf(pipe,"plot '-' \n");

      for(auto i{0} ; i<=nx ; i++){
         fprintf( pipe, "%d %f\n" , i , temp[i].v );
      }

      for(auto i{0} ; i<=nx ; i++){
         fprintf( pipe , "%d %f\n", i , temp_obs[i] );
      }
      /*Plotting code ends here.*/

      //reset the DCO tape
      dco_a1s_reset_tape();

      for(auto i{0};i<=nx;i++){
        temp[i] = temp[i].v;
      }

      fprintf(pipe, "e\n");
      //Update GNUPlot
      fflush(pipe);
      usleep(1000000);

      iters++;

  }while(grad_norm>epsilon && iters<=max_iterations);

  //Deallocate the DCO tape
  dco_a1s_destroy_tape();

  //Print out result
  std::cout << "\nSimulation Result: [" << iters << " iterations required]" << std::endl;
  std::cout << "Location" << "\t\t\t\t" << "Observed" << "\t\t\t\t" << "Computed" << std::endl;

  for(auto i{0} ; i<nx ; i++){
    std::cout  << i  << "\t\t\t\t" <<  temp_obs[i] << "\t\t\t\t" << temp[i].v << std::endl;
  }

  //Close pipe
  fclose(pipe);

  //Deallocate memory resources
  delete [] grad;
  delete [] temp_obs;
  delete [] temp_old; 
  delete [] temp; 
  		
  return 0;
}

