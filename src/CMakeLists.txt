cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -g -pthread")
project(2lab)
add_executable(os_lab_03 main.cpp)

