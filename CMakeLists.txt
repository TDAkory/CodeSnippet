cmake_minimum_required(VERSION 3.25)

project(CodeSnippet)

set(CMAKE_CXX_STANDARD 23)

set(SOURCE_FILES main.cpp)

add_executable(CodeSnippet_run ${SOURCE_FILES})

target_compile_features(CodeSnippet_run PUBLIC cxx_std_23)

add_subdirectory(container)
add_subdirectory(test)
add_subdirectory(bench)
add_subdirectory(thread_pool)
add_subdirectory(file_ops)
add_subdirectory(string_ops)
add_subdirectory(proc_ops)
add_subdirectory(timer)

include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
    set_property(TARGET CodeSnippet_run PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(WARNING "IPO is not supported: ${output}")
endif()