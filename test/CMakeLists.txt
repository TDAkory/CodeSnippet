cmake_minimum_required(VERSION 3.12)

project(CodeSnippet_tests)
add_subdirectory(lib)

INCLUDE_DIRECTORIES(../container)
INCLUDE_DIRECTORIES(../op_file)
INCLUDE_DIRECTORIES(../static_reflect)
INCLUDE_DIRECTORIES(../op_str)
INCLUDE_DIRECTORIES(../op_proc)

set(TEST_SRCS
        ../container/falgo_test.cpp
        ../op_file/FileOps_test.cpp
        ../op_proc/proc_util_test.cpp
        ../static_reflect/static_reflect_test.cpp
        ../op_str/StringSearch_test.cpp
        ../container/sync_pool_test.cpp
        ../container/bi_tree_printer_test.cpp
        ../op_func/scope_guard_test.cpp
)

add_definitions(-DCODESNIPPET_DO_TEST)

add_executable(CodeSnippet_tests test_main.cpp ${TEST_SRCS})

target_link_libraries(CodeSnippet_tests container_lib)
target_link_libraries(CodeSnippet_tests FileOps_lib)
target_link_libraries(CodeSnippet_tests StringOps_lib)
target_link_libraries(CodeSnippet_tests func_cache_lib)
target_link_libraries(CodeSnippet_tests ProcOps_lib)
target_link_libraries(CodeSnippet_tests gtest gtest_main)
