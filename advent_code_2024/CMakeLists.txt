cmake_minimum_required(VERSION 3.11)

project(advent_code_2024)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

set(SRCS main.cpp util.cpp)

add_executable(advent_code_2024 ${SRCS})