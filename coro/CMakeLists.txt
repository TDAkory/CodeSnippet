cmake_minimum_required(VERSION 3.12)

project(coro_util)

set(CMAKE_CXX_STANDARD 20)

file(GLOB EXAMPLE_INCLUDE LIST_DIRECTORIES false "example_*.h")

set(LIBRARY_INCLUDE result.h)

add_library(cs_coro INTERFACE)

find_package(httplib CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(coro_run coro_run.cpp)
target_link_libraries(coro_run cs_coro cs_util httplib::httplib nlohmann_json::nlohmann_json)

