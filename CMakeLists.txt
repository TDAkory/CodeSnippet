cmake_minimum_required(VERSION 3.22)
project(CodeSnippet)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp)

add_executable(CodeSnippet_run ${SOURCE_FILES})

add_subdirectory(container)
add_subdirectory(test)
add_subdirectory(bench)
add_subdirectory(thread_pool)
add_subdirectory(file_ops)
add_subdirectory(string_ops)
add_subdirectory(proc_ops)