//
// Created by zhaojieyi on 2022/5/16.
//
#include <iostream>
#include <thread>
#include "benchmark/benchmark.h"
#include "safe_vector.h"

using namespace std;
using namespace benchmark;
using namespace safe_container;

const int num = 50000;

LVector<int> *vec_loc;
LFVector<int> *vec_cas;

mutex mtx;

static void DoSetup(const benchmark::State &state) {
    vec_loc = new LVector<int>();
    vec_cas = new LFVector<int>();
}

static void DoTeardown(const benchmark::State &state) {
    delete vec_loc;
    delete vec_cas;
}

template <typename T>
void do_push_back(const int64_t range, T *vec) {
    for (int i = 0; i < range; i++) {
        vec->push_back(i);
    }
}

static void BM_VEC_LOC(benchmark::State &state) {
    for (auto _ : state) {
        do_push_back(num, vec_loc);
    }
}

static void BM_VEC_CAS(benchmark::State &state) {
    for (auto _ : state) {
        do_push_back(num, vec_cas);
    }
}

// Register the function as a benchmark
BENCHMARK(BM_VEC_LOC)->ThreadRange(1, 8)->Setup(DoSetup)->Teardown(DoTeardown);
BENCHMARK(BM_VEC_CAS)->ThreadRange(1, 8)->Setup(DoSetup)->Teardown(DoTeardown);
