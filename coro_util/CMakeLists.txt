cmake_minimum_required(VERSION 3.12)

project(coro_util)

set(CMAKE_CXX_STANDARD 23)

set(EXE_SOURCE_FILES coro_run.cpp)

file(GLOB EXAMPLE_INCLUDE LIST_DIRECTORIES false "example_\\d.h")

set(EXE_INCLUDE_FILES
        ${EXAMPLE_INCLUDE})

add_executable(coro_run ${EXE_SOURCE_FILES} ${EXE_INCLUDE_FILES})

target_link_libraries(coro_run Util_lib)