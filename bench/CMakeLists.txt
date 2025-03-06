project(CodeSnippet_bench)

find_package(benchmark CONFIG REQUIRED)

INCLUDE_DIRECTORIES(../container)

set(BENCH_SRCS
        bench_main.cpp
        ../container/falgo_bench.cpp
        ../container/safe_vector_bench.cpp
        ../container/sync_pool_bench.cpp
        ../op_str/string_split_bench.cpp
)

add_executable(CodeSnippet_bench ${BENCH_SRCS})

target_link_libraries(CodeSnippet_bench cs_container)
target_link_libraries(CodeSnippet_bench cs_str)
target_link_libraries(CodeSnippet_bench benchmark::benchmark benchmark::benchmark_main)