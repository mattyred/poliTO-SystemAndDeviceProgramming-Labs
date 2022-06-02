cmake_minimum_required(VERSION 3.15)
project(Exercise_C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

add_executable(Exercise_C main.cpp)