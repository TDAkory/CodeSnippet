cmake_minimum_required(VERSION 3.11)

project(advent_code_2024)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

set(Python3_EXECUTABLE /usr/bin/python3)

set(SRCS main.cpp 07.cpp)

add_executable(advent_code_2024 ${SRCS})