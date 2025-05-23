cmake_minimum_required(VERSION 3.12)

project(CodeSnippet_tests)

find_package(GTest CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

INCLUDE_DIRECTORIES(../container)
INCLUDE_DIRECTORIES(../op_file)
INCLUDE_DIRECTORIES(../static_reflect)
INCLUDE_DIRECTORIES(../op_str)
INCLUDE_DIRECTORIES(../op_proc)

set(TEST_SRCS
        ../container/falgo_test.cpp
        ../op_file/FileOps_test.cpp
        ../op_proc/proc_util_test.cpp
        ../op_proc/cmd_util_test.cpp
        ../static_reflect/static_reflect_test.cpp
        ../op_str/string_search_test.cpp
        ../container/sync_pool_test.cpp
        ../container/bi_tree_printer_test.cpp
        ../op_func/scope_guard_test.cpp
        ../op_str/string_split_test.cpp
)

add_definitions(-DCODESNIPPET_DO_TEST)

add_executable(CodeSnippet_tests test_main.cpp ${TEST_SRCS})

target_link_libraries(CodeSnippet_tests cs_container)
target_link_libraries(CodeSnippet_tests cs_file)
target_link_libraries(CodeSnippet_tests cs_str)
target_link_libraries(CodeSnippet_tests cs_func)
target_link_libraries(CodeSnippet_tests cs_proc)
target_link_libraries(CodeSnippet_tests GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

target_compile_options(CodeSnippet_tests PRIVATE -fno-access-control)

#add_subdirectory(module_test)