#include <cmath>
#include <iostream>
#include <vector>

#include "dco_a1s_type.hpp"

using namespace std;

int dco_a1s_vac {1};

dco_a1s_tape_entry * dco_a1s_tape;

vector<int> dco_a1s_indep_va;
vector<int> dco_a1s_dep_va;

dco_a1s_type::dco_a1s_type():
va(0),
v(0.0)
{ 
  
}

dco_a1s_type::dco_a1s_type( const double & x )
{
  v = x;
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_CONST;
  dco_a1s_tape[dco_a1s_vac].v = x;
  va = dco_a1s_vac++;

  #ifdef DCO_A1S_DEBUG
  std::cout << "dco_a1s_type( const double & )" << std::endl;
  #endif
}

dco_a1s_type::dco_a1s_type( const dco_a1s_type & x )
{
  if(this!=&x)
  {
    v = x.v;
    dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_ASG;
    dco_a1s_tape[dco_a1s_vac].v = v;
    dco_a1s_tape[dco_a1s_vac].arg1 = x.va;
    va = dco_a1s_vac++;
  }

  #ifdef DCO_A1S_DEBUG
  std::cout << "dco_a1s_type( const dco_a1s_type & )" << std::endl;
  #endif
}

dco_a1s_type & dco_a1s_type::operator=( const dco_a1s_type & x )
{
  if(this==&x)
    return *this;

  v = x.v;
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_ASG;
  dco_a1s_tape[dco_a1s_vac].v = v;
  dco_a1s_tape[dco_a1s_vac].arg1 = x.va;
  va = dco_a1s_vac++;

  #ifdef DCO_A1S_DEBUG
  std::cout << "operator=( const dco_a1s_type & )" << std::endl;
  #endif

  return *this;
}

/*dco_a1s_type & dco_a1s_type::operator=( double x )
{
  v = x;
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_CONST;
  dco_a1s_tape[dco_a1s_vac].v = x;
  va = dco_a1s_vac++;
  std::cout << "operator=( double )" << std::endl;
  return *this;
}*/

dco_a1s_type & dco_a1s_type::operator+=( double x )
{
   v += x;
   dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_ASG;
   dco_a1s_tape[dco_a1s_vac].v = v;
   dco_a1s_tape[dco_a1s_vac].arg1 = va;
   va = dco_a1s_vac++;

  #ifdef DCO_A1S_DEBUG
  std::cout << "operator+=( double )" std::endl;
  #endif

   return *this;
}

dco_a1s_type & dco_a1s_type::operator+=( const dco_a1s_type & x )
{
  v += x.v;
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_AA_ADD;
  dco_a1s_tape[dco_a1s_vac].v = v;
  dco_a1s_tape[dco_a1s_vac].arg1 = va;
  dco_a1s_tape[dco_a1s_vac].arg2 = x.va;
  va = dco_a1s_vac++;
  return *this;
}

dco_a1s_type & dco_a1s_type::operator-=( double x )
{
  v -= x;
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_ASG;
  dco_a1s_tape[dco_a1s_vac].v = v;
  dco_a1s_tape[dco_a1s_vac].arg1 = va;
  va = dco_a1s_vac++;
  return *this;
}

dco_a1s_type & dco_a1s_type::operator-=( const dco_a1s_type & x )
{
  v -= x.v;
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_AA_SUB;
  dco_a1s_tape[dco_a1s_vac].v = v;
  dco_a1s_tape[dco_a1s_vac].arg1 = va;
  dco_a1s_tape[dco_a1s_vac].arg2 = x.va;
  va = dco_a1s_vac++;
  return *this;
}

dco_a1s_type operator*( const dco_a1s_type & x1 , const dco_a1s_type & x2 )
{
  dco_a1s_type tmp;
  tmp.v = x1.v*x2.v;
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_AA_MUL;
  dco_a1s_tape[dco_a1s_vac].arg1 = x1.va;
  dco_a1s_tape[dco_a1s_vac].arg2 = x2.va;
  dco_a1s_tape[dco_a1s_vac].v = tmp.v;
  tmp.va = dco_a1s_vac++;

  #ifdef DCO_A1S_DEBUG
  std::cout << "operator*( const dco_a1s_type & , const dco_a1s_type & )" << std::endl;
  #endif

  return tmp;
}

dco_a1s_type operator*( const dco_a1s_type & x1 , double x2 )
{
  dco_a1s_type tmp;
  tmp.v = x1.v*x2;
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_AP_MUL;
  dco_a1s_tape[dco_a1s_vac].arg1 = x1.va;
  dco_a1s_tape[dco_a1s_vac].v = tmp.v;
  dco_a1s_tape[dco_a1s_vac].val2 = x2;
  tmp.va = dco_a1s_vac++;
  return tmp;
}

dco_a1s_type operator*( double x1 , const dco_a1s_type & x2 )
{
  dco_a1s_type tmp;
  tmp.v = x1*x2.v;
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_PA_MUL;
  dco_a1s_tape[dco_a1s_vac].arg2 = x2.va;
  dco_a1s_tape[dco_a1s_vac].v = tmp.v;
  dco_a1s_tape[dco_a1s_vac].val1 = x1;
  tmp.va = dco_a1s_vac++;
  return tmp;
}

dco_a1s_type operator/( const dco_a1s_type & x1 , const dco_a1s_type & x2 )
{
  dco_a1s_type tmp;
  tmp.v = x1.v / x2.v;
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_AA_DIV;
  dco_a1s_tape[dco_a1s_vac].arg1 = x1.va;
  dco_a1s_tape[dco_a1s_vac].arg2 = x2.va;
  dco_a1s_tape[dco_a1s_vac].v = tmp.v;
  tmp.va = dco_a1s_vac++;
  return tmp;
}

dco_a1s_type operator/( const dco_a1s_type & x1 , double  x2 )
{
  dco_a1s_type tmp;
  tmp.v = x1.v / x2;
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_AP_DIV;
  dco_a1s_tape[dco_a1s_vac].arg1 = x1.va;
  dco_a1s_tape[dco_a1s_vac].v = tmp.v;
  dco_a1s_tape[dco_a1s_vac].val2 = x2;//x2 is needed for the calculation of the partial derivative
  tmp.va = dco_a1s_vac++;
  return tmp;
}

dco_a1s_type operator/( double x1 , const dco_a1s_type & x2 )
{
  dco_a1s_type tmp;
  tmp.v = x1/ x2.v;
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_PA_DIV;
  dco_a1s_tape[dco_a1s_vac].arg2 = x2.va;
  dco_a1s_tape[dco_a1s_vac].v = tmp.v;
  dco_a1s_tape[dco_a1s_vac].val1 = x1;
  dco_a1s_tape[dco_a1s_vac].val2 = x2.v;
  tmp.va = dco_a1s_vac++;
  return tmp;
}

dco_a1s_type operator+( const dco_a1s_type & x1 , const dco_a1s_type & x2 )
{
  dco_a1s_type tmp;
  tmp.v = x1.v+x2.v;
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_AA_ADD;
  dco_a1s_tape[dco_a1s_vac].arg1 = x1.va;
  dco_a1s_tape[dco_a1s_vac].arg2 = x2.va;
  dco_a1s_tape[dco_a1s_vac].v = tmp.v;
  tmp.va = dco_a1s_vac++;

  #ifdef DCO_A1S_DEBUG
  std::cout << "operator+( const dco_a1s_type & , const dco_a1s_type & )" << std::endl;
  #endif

  return tmp;
}

dco_a1s_type operator+( const dco_a1s_type & x1 , double x2 )
{
  dco_a1s_type tmp;
  tmp.v = x1.v+x2;
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_AP_ADD;
  dco_a1s_tape[dco_a1s_vac].arg1 = x1.va;
  dco_a1s_tape[dco_a1s_vac].v = tmp.v;
  tmp.va = dco_a1s_vac++;

  #ifdef DCO_A1S_DEBUG
  std::cout << "operator+( const dco_a1s_type & , double )" << std::endl;
  #endif

  return tmp;
}

dco_a1s_type operator+( double x1 , const dco_a1s_type & x2 )
{
  dco_a1s_type tmp;
  tmp.v = x1+x2.v;
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_PA_ADD;
  dco_a1s_tape[dco_a1s_vac].arg2 = x2.va;
  dco_a1s_tape[dco_a1s_vac].v = tmp.v;
  tmp.va = dco_a1s_vac++;

  #ifdef DCO_A1S_DEBUG
  std::cout << "operator+( double , const dco_a1s_type & )" << std::endl;
  #endif

  return tmp;
}

dco_a1s_type operator-( const dco_a1s_type & x1 , const dco_a1s_type & x2 )
{
  dco_a1s_type tmp;
  tmp.v = x1.v-x2.v;
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_AA_SUB;
  dco_a1s_tape[dco_a1s_vac].arg1 = x1.va;
  dco_a1s_tape[dco_a1s_vac].arg2 = x2.va;
  dco_a1s_tape[dco_a1s_vac].v = tmp.v;
  tmp.va = dco_a1s_vac++;

  #ifdef DCO_A1S_DEBUG
  std::cout << "operator-( const dco_a1s_type & , const dco_a1s_type & )" << std::endl;
  #endif

  return tmp;
}

dco_a1s_type operator-( const dco_a1s_type & x1 , double x2 )
{
  dco_a1s_type tmp;
  tmp.v = x1.v-x2;
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_AP_SUB;
  dco_a1s_tape[dco_a1s_vac].arg1 = x1.va;
  dco_a1s_tape[dco_a1s_vac].v = tmp.v;
  tmp.va = dco_a1s_vac++;

 #ifdef DCO_A1S_DEBUG
 std::cout << "operator-( const dco_a1s_type & , double )" << std::endl;
 #endif

  return tmp;
}

dco_a1s_type operator-( double x1 , const dco_a1s_type & x2 )
{
  dco_a1s_type tmp;
  tmp.v = x1-x2.v;
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_PA_SUB;
  dco_a1s_tape[dco_a1s_vac].arg2 = x2.va;
  dco_a1s_tape[dco_a1s_vac].v = tmp.v;
  tmp.va = dco_a1s_vac++;

 #ifdef DCO_A1S_DEBUG
 std::cout << "operator-( double , const dco_a1s_type & )" << std::endl;
 #endif

  return tmp;
}

dco_a1s_type operator-( const dco_a1s_type & x )
{
  dco_a1s_type tmp;
  tmp.v = -x.v;
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_MINUS;
  dco_a1s_tape[dco_a1s_vac].arg1 = x.va;
  dco_a1s_tape[dco_a1s_vac].v = tmp.v;
  tmp.va = dco_a1s_vac++;

  #ifdef DCO_A1S_DEBUG
  std::cout << "operator-( const dco_a1s_type & )" << std::endl;
  #endif

  return tmp;
}

dco_a1s_type sin( const dco_a1s_type & x )
{
  dco_a1s_type tmp;
  tmp.v = std::sin(x.v);
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_SIN;
  dco_a1s_tape[dco_a1s_vac].arg1 = x.va;
  dco_a1s_tape[dco_a1s_vac].v = tmp.v;
  tmp.va = dco_a1s_vac++;

  #ifdef DCO_A1S_DEBUG
  std::cout << "sin( const dco_a1s_type & )" << std::endl;
  #endif

  return tmp;
}

dco_a1s_type cos( const dco_a1s_type & x )
{
  dco_a1s_type tmp;
  tmp.v = std::cos(x.v);
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_COS;
  dco_a1s_tape[dco_a1s_vac].arg1 = x.va;
  dco_a1s_tape[dco_a1s_vac].v = tmp.v;
  tmp.va = dco_a1s_vac++;

  #ifdef DCO_A1S_DEBUG
  std::cout << "cos( const dco_a1s_type & )" << std::endl;
  #endif

  return tmp;
}

dco_a1s_type exp( const dco_a1s_type & x )
{
  dco_a1s_type tmp;
  tmp.v = std::exp(x.v);
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_EXP;
  dco_a1s_tape[dco_a1s_vac].arg1 = x.va;
  dco_a1s_tape[dco_a1s_vac].v = tmp.v;
  tmp.va = dco_a1s_vac++;

 #ifdef DCO_A1S_DEBUG
 std::cout << "exp( const dco_a1s_type & )" << std::endl;
 #endif

  return tmp;
}

dco_a1s_type log( const dco_a1s_type & x )
{
  dco_a1s_type tmp;
  tmp.v = std::log(x.v);
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_LOG;
  dco_a1s_tape[dco_a1s_vac].arg1 = x.va;
  dco_a1s_tape[dco_a1s_vac].v   = tmp.v;
  tmp.va = dco_a1s_vac++;

  #ifdef DCO_A1S_DEBUG
  std::cout << "log( const dco_a1s_type & )" << std::endl;
  #endif

  return tmp;
}

dco_a1s_type log10( const dco_a1s_type & x )
{
  dco_a1s_type tmp;
  tmp.v = std::log10(x.v);
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_LOG10;
  dco_a1s_tape[dco_a1s_vac].arg1 = x.va;
  dco_a1s_tape[dco_a1s_vac].v   = tmp.v;
  tmp.va = dco_a1s_vac++;

  #ifdef DCO_A1S_DEBUG
  std::cout << "log10( const dco_a1s_type & )" << std::endl;
  #endif

  return tmp;
}

dco_a1s_type tan( const dco_a1s_type & x )
{
  dco_a1s_type tmp;
  tmp.v = std::tan(x.v);
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_TAN;
  dco_a1s_tape[dco_a1s_vac].arg1 = x.va;
  dco_a1s_tape[dco_a1s_vac].v = tmp.v;
  tmp.va = dco_a1s_vac++;

  #ifdef DCO_A1S_DEBUG
  std::cout << "tan( const dco_a1s_type & )" << std::endl;
  #endif

  return tmp;
}


dco_a1s_type sqrt( const dco_a1s_type & x )
{
  dco_a1s_type tmp;
  tmp.v = std::sqrt(x.v);
  dco_a1s_tape[dco_a1s_vac].oc = DCO_A1S_SQRT;
  dco_a1s_tape[dco_a1s_vac].arg1 = x.va;
  dco_a1s_tape[dco_a1s_vac].v = tmp.v;
  tmp.va = dco_a1s_vac++;

  #ifdef DCO_A1S_DEBUG
  std::cout << "sqrt( const dco_a1s_type & )" << std::endl;
  #endif

  return tmp;
}

double abs( const dco_a1s_type & x )
{
  return std::abs(x.v);
}

bool operator==(const dco_a1s_type & x1 , const dco_a1s_type & x2 )
{
  return x1.v == x2.v;
}

bool operator==( const dco_a1s_type & x1 , double x2 )
{
  return x1.v < x2;
}

bool operator==( double x1 , const dco_a1s_type & x2 )
{
  return x1 < x2.v;
}

bool operator!=(const dco_a1s_type & x1 , const dco_a1s_type & x2 )
{
  return x1.v != x2.v;
}

bool operator!=( const dco_a1s_type & x1 , double x2 )
{
  return x1.v != x2;
}

bool operator!=( double x1 , const dco_a1s_type & x2 )
{
  return x1 != x2.v;
}

bool operator<( const dco_a1s_type & x1 , const dco_a1s_type & x2 )
{
  return x1.v < x2.v;
}

bool operator<( const dco_a1s_type & x1 , double x2 )
{
  return x1.v < x2;
}

bool operator<( double x1 , const dco_a1s_type & x2 )
{
  return x1 < x2.v;
}

bool operator<=( const dco_a1s_type & x1 , const dco_a1s_type & x2 )
{
  return x1.v <= x2.v;
}

bool operator<=( const dco_a1s_type & x1 , double x2 )
{
  return x1.v <= x2;
}

bool operator<=( double x1 , const dco_a1s_type & x2 )
{
  return x1 <= x2.v;
}

bool operator>( const dco_a1s_type & x1 , const dco_a1s_type & x2 )
{
  return x1.v > x2.v;
}

bool operator>( const dco_a1s_type & x1 , double x2 )
{
  return x1.v > x2;
}

bool operator>( double x1 , const dco_a1s_type & x2 )
{
  return x1 > x2.v;
}

bool operator>=( const dco_a1s_type & x1 , const dco_a1s_type & x2 )
{
  return x1.v >= x2.v;
}

bool operator>=( const dco_a1s_type & x1 , double x2 )
{
  return x1.v >= x2;
}

bool operator>=( double x1 , const dco_a1s_type & x2 )
{
  return x1 >= x2.v;
}

istream & operator>>( istream & in , const dco_a1s_type & x )
{
  in >> x.v;
  return in;
}

void dco_a1s_create_tape( int size )
{
  dco_a1s_tape = new dco_a1s_tape_entry[size];
}

void dco_a1s_destroy_tape()
{
  delete [] dco_a1s_tape;
}

void dco_a1s_register_independent( const dco_a1s_type & x )
{
  dco_a1s_indep_va.push_back(x.va);
}

void dco_a1s_register_dependent( const dco_a1s_type & x )
{
  dco_a1s_dep_va.push_back(x.va);
}

void dco_a1s_print_tape()
{
  cout << "TAPE:" << endl;
  
  for( int i=0 ; i<dco_a1s_vac ; i++ )
  {
    cout << i << ": [ " << dco_a1s_tape[i].oc << " , "
    << dco_a1s_tape[i].arg1 << " , "
    << dco_a1s_tape[i].arg2 << " , "
    << dco_a1s_tape[i].v << " , "
    << dco_a1s_tape[i].a << " ] " << endl;
  }
}

void dco_a1s_reset_tape()
{
  for( auto i{0} ; i<dco_a1s_vac ; i++ ){
    dco_a1s_tape[i].a = 0;
  }

  dco_a1s_vac = 0;

  dco_a1s_indep_va.clear();
  dco_a1s_dep_va.clear();
}

void dco_a1s_interpret_tape()
{
  for( auto i{dco_a1s_vac-1} ; i>=0 ; i-- )
  {
    switch(dco_a1s_tape[i].oc)
    {
      case DCO_A1S_CONST:
      {
        dco_a1s_tape[i].a = 0.0;
        break;
      }

      case DCO_A1S_ASG:
      {
        dco_a1s_tape[dco_a1s_tape[i].arg1].a+=dco_a1s_tape[i].a;
        break;
      }

      case DCO_A1S_AA_ADD:
      {
        dco_a1s_tape[dco_a1s_tape[i].arg1].a+=dco_a1s_tape[i].a;
        dco_a1s_tape[dco_a1s_tape[i].arg2].a+=dco_a1s_tape[i].a;
        break;
      }

      case DCO_A1S_AP_ADD:
      {
        dco_a1s_tape[dco_a1s_tape[i].arg1].a+=dco_a1s_tape[i].a;
        break;
      }

      case DCO_A1S_PA_ADD:
      {
        dco_a1s_tape[dco_a1s_tape[i].arg2].a+=dco_a1s_tape[i].a;
	break;
      }

      case DCO_A1S_AA_SUB:
      {
        dco_a1s_tape[dco_a1s_tape[i].arg1].a+=dco_a1s_tape[i].a;
        dco_a1s_tape[dco_a1s_tape[i].arg2].a-=dco_a1s_tape[i].a;
        break; 
      }

      case DCO_A1S_AP_SUB:
      {
        dco_a1s_tape[dco_a1s_tape[i].arg1].a+=dco_a1s_tape[i].a;
 	break;
      }
	
      case DCO_A1S_PA_SUB:
      {
        dco_a1s_tape[dco_a1s_tape[i].arg2].a-=dco_a1s_tape[i].a;
	break;
      }

      case DCO_A1S_MINUS:
      {
        dco_a1s_tape[dco_a1s_tape[i].arg1].a-=dco_a1s_tape[i].a;
      }

      case DCO_A1S_AA_MUL:
      {
        dco_a1s_tape[dco_a1s_tape[i].arg1].a+=dco_a1s_tape[dco_a1s_tape[i].arg2].v*dco_a1s_tape[i].a;
        dco_a1s_tape[dco_a1s_tape[i].arg2].a+=dco_a1s_tape[dco_a1s_tape[i].arg1].v*dco_a1s_tape[i].a;
        break;
      }
	
      case DCO_A1S_AP_MUL:
      {
	dco_a1s_tape[dco_a1s_tape[i].arg1].a+=(dco_a1s_tape[i].val2)*dco_a1s_tape[i].a;
	//dco_a1s_tape[dco_a1s_tape[i].arg1].a+=(dco_a1s_tape[i].v/dco_a1s_tape[dco_a1s_tape[i].arg1].v)*dco_a1s_tape[i].a;        
	break;
      }

      case DCO_A1S_PA_MUL:
      {
	dco_a1s_tape[dco_a1s_tape[i].arg2].a+=(dco_a1s_tape[i].val1)*dco_a1s_tape[i].a;
	//dco_a1s_tape[dco_a1s_tape[i].arg2].a+=(dco_a1s_tape[i].v/dco_a1s_tape[dco_a1s_tape[i].arg2].v)*dco_a1s_tape[i].a;        
	break;
      }

      case DCO_A1S_SIN:
      {
        dco_a1s_tape[dco_a1s_tape[i].arg1].a+=cos(dco_a1s_tape[dco_a1s_tape[i].arg1].v)*dco_a1s_tape[i].a;
        break;
      }

      case DCO_A1S_COS:
      {
        dco_a1s_tape[dco_a1s_tape[i].arg1].a-=sin(dco_a1s_tape[dco_a1s_tape[i].arg1].v)*dco_a1s_tape[i].a;
        break;
      }

      case DCO_A1S_EXP:
      {
        dco_a1s_tape[dco_a1s_tape[i].arg1].a+= exp(dco_a1s_tape[dco_a1s_tape[i].arg1].v)*dco_a1s_tape[i].a;
        break;
      }

      case DCO_A1S_LOG:
      {
        dco_a1s_tape[dco_a1s_tape[i].arg1].a+=(1.0/dco_a1s_tape[dco_a1s_tape[i].arg1].v)*dco_a1s_tape[i].a;
        break;
      }

      case DCO_A1S_LOG10:
      {
         dco_a1s_tape[dco_a1s_tape[i].arg1].a+=(1.0/(dco_a1s_tape[dco_a1s_tape[i].arg1].v*std::log(10)))*dco_a1s_tape[i].a;
         break;
      }

       case DCO_A1S_TAN:
       {
          dco_a1s_tape[dco_a1s_tape[i].arg1].a+=(1+(dco_a1s_tape[dco_a1s_tape[i].arg1].v)*(dco_a1s_tape[dco_a1s_tape[i].arg1].v))*dco_a1s_tape[i].a;
          break;
       }

       case DCO_A1S_AA_DIV:
       {
         dco_a1s_tape[dco_a1s_tape[i].arg1].a += (1.0/dco_a1s_tape[dco_a1s_tape[i].arg2].v)*dco_a1s_tape[i].a;
         dco_a1s_tape[dco_a1s_tape[i].arg2].a  -= (dco_a1s_tape[dco_a1s_tape[i].arg1].v/std::pow(dco_a1s_tape[dco_a1s_tape[i].arg2].v,2))
	 *dco_a1s_tape[i].a;
         break;
       }

       case DCO_A1S_AP_DIV:
       {
         dco_a1s_tape[dco_a1s_tape[i].arg1].a += (1.0/dco_a1s_tape[i].val2)*dco_a1s_tape[i].a;
         break;
       }

       case DCO_A1S_PA_DIV:
       {
          dco_a1s_tape[dco_a1s_tape[i].arg2].a -= (dco_a1s_tape[i].val1/std::pow(dco_a1s_tape[i].val2,2.0))*dco_a1s_tape[i].a;
          break;
       }

       case DCO_A1S_SQRT:
       {
          if(dco_a1s_tape[dco_a1s_tape[i].arg1].v>0.0)
            dco_a1s_tape[dco_a1s_tape[i].arg1].a+= 0.5*(1.0/std::sqrt(dco_a1s_tape[dco_a1s_tape[i].arg1].v))*dco_a1s_tape[i].a;
          else
            dco_a1s_tape[dco_a1s_tape[i].arg1].a+= 0.5*(1.0/std::sqrt(dco_a1s_tape[dco_a1s_tape[i].arg1].v+1.0e-12))*dco_a1s_tape[i].a;
          break;
       }
    }
  }
}

double dco_a1s_value( const dco_a1s_type & x )
{
  return x.v;
}

double dco_a1s_derivative( const int i )
{
  return dco_a1s_tape[dco_a1s_indep_va[i]].a;
}

double get_wall_time()
{
  struct timeval time;

  if(gettimeofday(&time,NULL)){
    return 0;
  }

  return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

double get_cpu_time()
{
  return (double)clock()/CLOCKS_PER_SEC;
}

