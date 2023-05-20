project(CodeSnippet_bench)

add_subdirectory(lib)

INCLUDE_DIRECTORIES(../container)

add_executable(CodeSnippet_bench_run
        falgo_bench.cpp
        safe_vector_bench.cpp)

target_link_libraries(CodeSnippet_bench_run container_lib)
target_link_libraries(CodeSnippet_bench_run benchmark::benchmark)