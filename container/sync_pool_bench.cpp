//
// Created by zhaojieyi on 2023/6/3.
//

#include "sync_pool.h"
#include <vector>
#include <string>
#include "benchmark/benchmark.h"

using namespace safe_container;

struct BenchModel {
    int8_t len_8{0};
    int32_t len_32{0};
    std::string str;
    std::vector<void *> vec;
};

void DoSomething(BenchModel *m) {
    m->len_8 = 0x4f;
    m->len_32 = 0x12314;
}

static void BM_NORMAL_NEW(benchmark::State &state) {
    for (auto _ : state) {
        auto *m = new BenchModel();
        DoSomething(m);
        delete m;
    }
}

static void BM_SYNCPOOL_NEW(benchmark::State &state) {
    SyncPool<BenchModel> pool(10);
    for (auto _ : state) {
        auto *m = pool.Get();
        DoSomething(m);
        pool.Put(m);
    }
}

BENCHMARK(BM_NORMAL_NEW)->ThreadRange(1, 16);
BENCHMARK(BM_SYNCPOOL_NEW)->ThreadRange(1, 16);
