project(CodeSnippet_tests)
add_subdirectory(lib)

INCLUDE_DIRECTORIES(../container)
INCLUDE_DIRECTORIES(../file_ops)
INCLUDE_DIRECTORIES(../static_reflect)
INCLUDE_DIRECTORIES(../string_ops)
INCLUDE_DIRECTORIES(../proc_ops)

add_executable(CodeSnippet_tests
        falgo_test.cpp
        FileOps_test.cpp
        ProcOps_test.cpp
        StaticReflect_test.cpp
        StringSearch_test.cpp)

target_link_libraries(CodeSnippet_tests container_lib)
target_link_libraries(CodeSnippet_tests FileOps_lib)
target_link_libraries(CodeSnippet_tests StringOps_lib)
#target_link_libraries(CodeSnippet_tests func_cache_lib)
target_link_libraries(CodeSnippet_tests ProcOps_lib)
target_link_libraries(CodeSnippet_tests gtest gtest_main)
