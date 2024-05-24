cmake_minimum_required(VERSION 3.12)

project(coro_util)

set(CMAKE_CXX_STANDARD 23)

file(GLOB EXAMPLE_INCLUDE LIST_DIRECTORIES false "example_*.h")

add_library(cs_coro INTERFACE ${EXAMPLE_INCLUDE})

add_executable(coro_run coro_run.cpp)
target_link_libraries(coro_run cs_coro Util_lib)