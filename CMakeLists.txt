cmake_minimum_required (VERSION 3.0)
project (odeSolver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -fopenmp")

add_executable(test "test.cpp")
add_executable(solve "main.cpp")
