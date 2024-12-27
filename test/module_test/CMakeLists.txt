cmake_minimum_required(VERSION 3.25)

project(module_test)

set(CMAKE_CXX_STANDARD 20)

add_library(hello)
target_sources(hello PUBLIC FILE_SET CXX_MODULES FILES hello.cpp)

add_executable(module_test main.cpp)
target_link_libraries(module_test hello)