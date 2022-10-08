project(CodeSnippet_tests)
add_subdirectory(lib)

INCLUDE_DIRECTORIES(../container)
INCLUDE_DIRECTORIES(../file_ops)

add_executable(CodeSnippet_tests falgo_test.cpp FileOps_test.cpp)

target_link_libraries(CodeSnippet_tests container_lib)
target_link_libraries(CodeSnippet_tests FileOps_lib)
target_link_libraries(CodeSnippet_tests gtest gtest_main)