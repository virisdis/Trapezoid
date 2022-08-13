// ------------------------------------------------------------------------------------------

#include <cmath>
#include <iostream>
#include <omp.h>

using namespace std;
#include "trapezoid.hpp"

// ------------------------------------------------------------------------------------------

int main()
{                                                                       
  cout << endl << trapezoid_seq([](auto x){return pow(x,2);}, -5., 5., 1000000) << endl;
  
  cout << endl << trapezoid_con([](auto x){return sin(12*x);}, 0., 10., 100000, 8) << endl;
}

// ------------------------------------------------------------------------------------------
