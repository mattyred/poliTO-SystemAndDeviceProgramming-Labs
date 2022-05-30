cmake_minimum_required(VERSION 3.15)
project(Exercise_A)

set(CMAKE_CXX_STANDARD 17)

add_executable(Exercise_A main.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")
