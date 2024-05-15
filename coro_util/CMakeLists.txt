cmake_minimum_required(VERSION 3.12)

project(coro_util)

set(CMAKE_CXX_STANDARD 23)

set(EXE_SOURCE_FILES coro_run.cpp)

set(EXE_INCLUDE_FILES example_1.h
        example_2.h)

add_executable(coro_run ${EXE_SOURCE_FILES} ${EXE_INCLUDE_FILES})