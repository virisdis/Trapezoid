// ------------------------------------------------------------------------------------------

#define BOOST_TEST_MODULE TestTrapezoid
#include <boost/test/included/unit_test.hpp>

#include <cmath>
#include <iostream>
#include <omp.h>

using namespace std;
#include "trapezoid.hpp"

// ------------------------------------------------------------------------------------------

template <typename T>
requires is_floating_point_v<T>
bool flPointEquality(T ex, T ac)
{
  return fabs(ex - ac) < 1e-6; // выбор 1e-6 несколько произволен
}

// ------------------------------------------------------------------------------------------

// данные на вход теста
// см. trapezoid.hh
#define MACRO_INIT_TEST_DATA_TRAPEZOID             \
  auto f = [](auto x){ return cos(x) + sin(x); };  \
  auto x1 = 0., x2 = 1.;                           \
  auto n = 10000000;                               \
  auto t = 4;                                      \
                                                   \
  auto ex = 1.301169; // expected

// ------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(expected_vs_actual) // проверка последовательного расчета (в сравнении с тестовыми данными)
{

  // ---------------
  
  MACRO_INIT_TEST_DATA_TRAPEZOID
  
  // ---------------
  
  auto ac = trapezoid_seq(f, x1, x2, n); // actual
  
  cout << endl << "Let's test correctness of sequential integration." << endl;
  
  cout << endl << "The expected value is " << ex << endl << "The actual value is " << ac << endl << endl;
  BOOST_CHECK(flPointEquality(ex, ac));
}

// ------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(sequential_vs_concurrent) // сравнение последовательного и параллельного расчетов 
{

  // ---------------
  
  MACRO_INIT_TEST_DATA_TRAPEZOID
  
  // ---------------
  
  cout << setprecision(10);
  
  // ---------------
  
  cout << endl << "Let's compare sequential with concurrent integration." << endl;
  
  auto begin_time = omp_get_wtime(); // начинаем отсчет
  auto seq = trapezoid_seq(f, x1, x2, n);
  cout << endl << "Result (sequiential): " << seq;
  auto end_time = omp_get_wtime(); // завершаем отсчет
  auto time_spent = (end_time - begin_time) * 1000; // потраченное время
  cout << endl << "Time: " << time_spent << " ms" << endl;
  
  begin_time = omp_get_wtime(); // начинаем отсчет
  auto con = trapezoid_con(f, x1, x2, n, t);
  cout << endl << "Result (concurrent): " << con;
  end_time = omp_get_wtime(); // завершаем отсчет
  time_spent = (end_time - begin_time) * 1000; // потраченное время
  cout << endl << "Time: " << time_spent << " ms" << endl;
  
  BOOST_CHECK(flPointEquality(seq, con));
}

// ------------------------------------------------------------------------------------------
