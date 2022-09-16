cmake_minimum_required(VERSION 3.22)
project(CodeSnippet)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp)

add_executable(CodeSnippet_run ${SOURCE_FILES})

include_directories(container)
add_subdirectory(container)
target_link_libraries(CodeSnippet_run container_lib)

add_subdirectory(test)
add_subdirectory(bench)
add_subdirectory(thread_pool)