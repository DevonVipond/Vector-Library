cmake_minimum_required(VERSION 3.8)
project(Vector_Library_CPP)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES library.cpp library.h)
add_library(Vector_Library_CPP ${SOURCE_FILES})