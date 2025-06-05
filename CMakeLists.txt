cmake_minimum_required(VERSION 4.0)
project(GraphGen)

set(CMAKE_CXX_STANDARD 20)

add_executable(GraphGen main.cpp graphGen.hpp testlib.h)
