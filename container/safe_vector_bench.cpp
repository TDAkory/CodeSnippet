//
// Created by zhaojieyi on 2022/5/16.
//
#include<iostream>
#include "benchmark/benchmark.h"
#include "safe_vector.h"

#include<mutex>
#include<thread>

using namespace std;
using namespace benchmark;
using namespace safe_container;

const int num = 50000;

TLVector<int> *vec_thl;
LVector<int> *vec_loc;
LFVector<int> *vec_cas;

mutex mtx;

static void DoSetup(const benchmark::State& state) {
    vec_thl = new TLVector<int>();
    vec_loc = new LVector<int>();
    vec_cas = new LFVector<int>();
}

static void DoTeardown(const benchmark::State& state) {
    delete vec_thl;
    delete vec_loc;
    delete vec_cas;
}

template<typename T>
void do_push_back(const int64_t range, T *vec) {
    for(int i =0; i< range; i++) {
        vec->push_back(i);
    }
}


static void BM_VEC_LOC(benchmark::State& state) {
    for(auto _: state){
        do_push_back(num, vec_loc);
    }
}


static void BM_VEC_CAS(benchmark::State& state) {
    for(auto _: state){
        do_push_back(num, vec_cas);
    }
}

static void BM_VEC_THL(benchmark::State& state) {
    for(auto _: state){
        do_push_back(num, vec_thl);
        {
            mtx.lock();
            vec_thl->merge();
            mtx.unlock();
        }
    }
}


// Register the function as a benchmark
BENCHMARK(BM_VEC_LOC)->Threads(10)->Threads(2)->Setup(DoSetup)->Teardown(DoTeardown);
BENCHMARK(BM_VEC_CAS)->Threads(10)->Threads(2)->Setup(DoSetup)->Teardown(DoTeardown);
//BENCHMARK(BM_VEC_THL)->Threads(10)->Threads(2)->Setup(DoSetup)->Teardown(DoTeardown);
