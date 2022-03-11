cmake_minimum_required(VERSION 3.21)
project(task_3)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)
include_directories(compact_vector)

add_executable(task_3 main.cpp rank_support.hpp sparse_array.hpp)
