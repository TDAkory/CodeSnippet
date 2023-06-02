cmake_minimum_required(VERSION 3.25)

project(CodeSnippet)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp)

add_executable(CodeSnippet_run ${SOURCE_FILES})

target_compile_features(CodeSnippet_run PUBLIC cxx_std_20)

add_subdirectory(test)
add_subdirectory(bench)
add_subdirectory(container)
add_subdirectory(mm)
add_subdirectory(op_file)
add_subdirectory(op_func)
add_subdirectory(op_str)
add_subdirectory(op_proc)
add_subdirectory(static_reflect)
add_subdirectory(thread_pool)
add_subdirectory(timer)

include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
    set_property(TARGET CodeSnippet_run PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(WARNING "IPO is not supported: ${output}")
endif()