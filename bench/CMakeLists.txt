project(CodeSnippet_bench)

add_subdirectory(lib)

INCLUDE_DIRECTORIES(../container)

set(BENCH_SRCS
        bench_main.cpp
        ../container/falgo_bench.cpp
        ../container/safe_vector_bench.cpp
        ../container/sync_pool_bench.cpp
        ../container/bi_tree_printer.h)

add_executable(CodeSnippet_bench ${BENCH_SRCS})

target_link_libraries(CodeSnippet_bench container_lib)
target_link_libraries(CodeSnippet_bench benchmark::benchmark)