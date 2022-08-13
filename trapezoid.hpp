// ------------------------------------------------------------------------------------------

#pragma once

#include <concepts>
#include <numeric>
#include <vector>
#include <omp.h>
#include <type_traits>

// ------------------------------------------------------------------------------------------

// requires is_floating_point_v<T>
// потребуем, чтобы аргументы, задающие пределы интегрирования,
// были числами с плавающей точкой

// ------------------------------------------------------------------------------------------

// f - интегрируемая функция
// x1, x2 - пределы интегрирования
// n - число узлов
// t - число потоков

// ------------------------------------------------------------------------------------------

template <typename F, typename T>
requires is_floating_point_v<T>
T trapezoid_seq(F f, const T x1, const T x2, const unsigned n)
{

  const T h = (x2 - x1)/n; // шаг интегрирования
  T sum = 0; // в цикле считаем сумму вертикалей

  for (unsigned i = 0; i < n; i++) // цикл
  {
    sum += f(x1 + i*h);
  }

  return h*sum - 0.5*h*f(x1) + 0.5*h*f(x2); // формула такова для единообразия с trapezoid_con()
}

// ------------------------------------------------------------------------------------------

template <typename F, typename T>
requires is_floating_point_v<T>
T trapezoid_con(F f, const T x1, const T x2, const unsigned long n, const unsigned t)
{

  const T h = (x2 - x1)/n; // шаг интегрирования
  T sum = 0; // в цикле считаем сумму вертикалей

  const unsigned nt = n/t; // целочисленное деление для распределения узлов по потокам
  auto partSum = vector<T>(t + 1, 0); // в параллельном цикле считаем частичную сумму вертикалей, не попавшие в цикл узлы соберем в последний элемент partSum
  
  #pragma omp parallel for num_threads(t)
  for (unsigned i = 0; i < t; i++) // параллельный цикл
  {
    auto thrNum = omp_get_thread_num();
    for (unsigned j = thrNum*nt; j < (thrNum+1)*nt; j++)
    {
      partSum[i] += f(x1 + j*h);
    }
  }
  
  for (unsigned k = t*nt; k < n; k++) // досчитаем узлы, не попавшие в параллельный цикл (число узлов ~ числу потоков)
  {
    partSum[t] += f(x1 + k*h);
  }

  sum = accumulate(partSum.begin(), partSum.end(), static_cast<T>(0)); // преобразование типа 0 к T (для корректности accumulate)
  
  return h*sum - 0.5*h*f(x1) + 0.5*h*f(x2); // формула такова для удобства распараллеливания
}

// ------------------------------------------------------------------------------------------
