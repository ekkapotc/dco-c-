#ifndef DCO_A1S_INCLUDED_
#define DCO_A1S_INCLUDED_

#include <vector>
#include <sys/time.h>
#include <cmath>

using namespace std;

#define DCO_A1S_TAPE_SIZE 	100000000

#define DCO_A1S_UNDEF 		-1

#define DCO_A1S_CONST 		0

#define DCO_A1S_ASG   		1

#define DCO_A1S_AA_ADD		2
#define DCO_A1S_AP_ADD  		3
#define DCO_A1S_PA_ADD  		4

#define DCO_A1S_AA_SUB		5
#define DCO_A1S_AP_SUB  		6
#define DCO_A1S_PA_SUB  		7

#define DCO_A1S_AA_MUL		8
#define DCO_A1S_AP_MUL  		9
#define DCO_A1S_PA_MUL  		10

#define DCO_A1S_SIN			11
#define DCO_A1S_COS			12
#define DCO_A1S_EXP			13
#define DCO_A1S_MINUS   		14

#define DCO_A1S_P_INC			15
#define DCO_A1S_A_INC			16

#define DCO_A1S_P_DEC   		17
#define DCO_A1S_A_DEC		18

#define DCO_A1S_LOG 			19
#define DCO_A1S_LOG10		20
#define DCO_A1S_TAN			21

#define DCO_A1S_AA_DIV 		22
#define DCO_A1S_AP_DIV		23
#define DCO_A1S_PA_DIV		24

#define DCO_A1S_SQRT			25

extern int dco_a1s_vac;
extern vector<int> dco_a1s_indep_va;
extern vector<int> dco_a1s_dep_va;

class dco_a1s_tape_entry
{
public:

  int oc;
  int arg1;
  int arg2;
  double v;
  double a;
  double val1;
  double val2;

public:
  dco_a1s_tape_entry():
  oc(DCO_A1S_UNDEF),
  arg1(0),
  arg2(0),
  v(0),
  a(0),
  val1(0),
  val2(0)
  { }
};

class dco_a1s_type
{
public:
  mutable int va;
  mutable double v;

public:
  dco_a1s_type();

  dco_a1s_type( const double & );
  
  dco_a1s_type( const dco_a1s_type & );

  dco_a1s_type & operator=( const dco_a1s_type & );

 // dco_a1s_type & operator=( double );

  dco_a1s_type & operator+=( double );

  dco_a1s_type & operator+=( const dco_a1s_type & );

  dco_a1s_type & operator-=( double );

  dco_a1s_type & operator-=( const dco_a1s_type & );
};

dco_a1s_type operator*( const dco_a1s_type & , const dco_a1s_type & );
dco_a1s_type operator*( const dco_a1s_type & , double );
dco_a1s_type operator*( double , const dco_a1s_type & );

dco_a1s_type operator/( const dco_a1s_type & , const dco_a1s_type & );
dco_a1s_type operator/( const dco_a1s_type & , double );
dco_a1s_type operator/( double , const dco_a1s_type & );

dco_a1s_type operator+( const dco_a1s_type & , const dco_a1s_type & );
dco_a1s_type operator+( const dco_a1s_type & , double );
dco_a1s_type operator+( double , const dco_a1s_type & );

dco_a1s_type operator-( const dco_a1s_type & , const dco_a1s_type & );
dco_a1s_type operator-( const dco_a1s_type & , double );
dco_a1s_type operator-( double , const dco_a1s_type & );
dco_a1s_type operator-( const dco_a1s_type & );

dco_a1s_type sin( const dco_a1s_type & );
dco_a1s_type cos( const dco_a1s_type & );
dco_a1s_type exp( const dco_a1s_type & );
dco_a1s_type log( const dco_a1s_type & );
dco_a1s_type log10( const dco_a1s_type & );
dco_a1s_type tan( const dco_a1s_type & );
dco_a1s_type sqrt( const dco_a1s_type & );

double abs( const dco_a1s_type & );

bool operator==( const dco_a1s_type & , const dco_a1s_type & );
bool operator==( const dco_a1s_type & , double );
bool operator==( double , const dco_a1s_type & );

bool operator!=( const dco_a1s_type & , const dco_a1s_type & );
bool operator!=( const dco_a1s_type & , double );
bool operator!=( double , const dco_a1s_type & );

bool operator<( const dco_a1s_type & , const dco_a1s_type & );
bool operator<( const dco_a1s_type & , double );
bool operator<( double , const dco_a1s_type & );

bool operator<=( const dco_a1s_type & , const dco_a1s_type & );
bool operator<= ( const dco_a1s_type & , double );
bool operator<=( double , const dco_a1s_type & );

bool operator>( const dco_a1s_type & , const dco_a1s_type & );
bool operator>( const dco_a1s_type & , double );
bool operator>( double , const dco_a1s_type & );

bool operator>=( const dco_a1s_type & , const dco_a1s_type & );
bool operator>=( const dco_a1s_type & , double );
bool operator>=( double , const dco_a1s_type & );

istream & operator>>(istream & in , const  dco_a1s_type & x );

void dco_a1s_create_tape( int size );
void dco_a1s_destroy_tape();
void dco_a1s_register_independent( const dco_a1s_type & );
void dco_a1s_register_dependent( const dco_a1s_type & );
void dco_a1s_print_tape();
void dco_a1s_interpret_tape();
void dco_a1s_reset_tape();

double dco_a1s_value( const dco_a1s_type & );
double dco_a1s_derivative( const int i );

double get_wall_time();
double get_cpu_time();

#endif
