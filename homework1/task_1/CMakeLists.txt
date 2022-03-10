cmake_minimum_required(VERSION 3.21)
project(task_1)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)
include_directories(compact_vector)

add_executable(task_1 main.cpp rank_support.hpp)
