project(CodeSnippet_bench)

add_subdirectory(lib)

INCLUDE_DIRECTORIES(../container)

set(BENCH_SRCS
        ../container/falgo_bench.cpp
        ../container/safe_vector_bench.cpp)

add_executable(CodeSnippet_bench_run ${BENCH_SRCS})

target_link_libraries(CodeSnippet_bench_run container_lib)
target_link_libraries(CodeSnippet_bench_run benchmark::benchmark)