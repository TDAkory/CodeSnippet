//
// Created by ByteDance on 2025/3/6.
//

#include "benchmark/benchmark.h"
#include "string_split.h"

static std::string g_origin = "hellp^world^there^are^string^util_functions^for^doSomeSplit^123xcv8909()^cmodf912";
static char g_delimiter = '^';

static void BM_SPLITBYFIND(benchmark::State &state) {
    for (auto _ : state) {
        auto ret = SplitByFind(g_origin, g_delimiter);
        benchmark::DoNotOptimize(ret);
    }
}

static void BM_SPLITBYGETLINE(benchmark::State &state) {
    for (auto _ : state) {
        auto ret = SplitByGetline(g_origin, g_delimiter);
        benchmark::DoNotOptimize(ret);
    }
}

static void BM_SPLITC20(benchmark::State &state) {
    for (auto _ : state) {
        auto ret = SplitC20(g_origin, g_delimiter);
        benchmark::DoNotOptimize(ret);
    }
}

BENCHMARK(BM_SPLITBYFIND);
BENCHMARK(BM_SPLITBYGETLINE);
BENCHMARK(BM_SPLITC20);