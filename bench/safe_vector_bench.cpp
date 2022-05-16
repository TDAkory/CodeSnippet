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

void push_back_lock(const int range, LVector<int> *vec_loc){
    for(int i = 0 ;i < range; i++){
        vec_loc->push_back(i);
    }
}

static void BM_VEC_LOC(benchmark::State& state) {
    const int num_th = state.range(0);
    int rg = num/num_th;
    thread th[num_th];


    for(auto _: state){
        for(int i = 0; i < num_th; i++){
            th[i] = thread(push_back_lock, rg, vec_loc);
        }

        for(int i = 0; i < num_th; i++){
            th[i].join();
        }
    }
}


void push_back_cas(const int range, LFVector<int> *vec_cas){
    for(int i = 0 ;i < range; i++){
        vec_cas->push_back(i);
    }
}

static void BM_VEC_CAS(benchmark::State& state) {
    const int num_th = state.range(0);
    int rg = num/num_th;
    thread th[num_th];


    for(auto _: state){
        for(int i = 0; i < num_th; i++){
            th[i] = thread(push_back_cas, rg, vec_cas);
        }

        for(int i = 0; i < num_th; i++){
            th[i].join();
        }
    }
}

void push_back_thl(const int range, TLVector<int> *vec_thl){
    for(int i = 0 ;i < range; i++){
        vec_thl->push_back(i);
    }

    {
        mtx.lock();
        vec_thl->merge();
        mtx.unlock();
    }
}

static void BM_VEC_THL(benchmark::State& state) {
    const int num_th = state.range(0);
    int rg = num/num_th;
    thread th[num_th];


    for(auto _: state){
        for(int i = 0; i < num_th; i++){
            th[i] = thread(push_back_thl, rg, vec_thl);
        }

        for(int i = 0; i < num_th; i++){
            th[i].join();
        }
    }
}


// Register the function as a benchmark
BENCHMARK(BM_VEC_LOC)->Arg(10)->Arg(2)->Setup(DoSetup)->Teardown(DoTeardown);
BENCHMARK(BM_VEC_CAS)->Arg(10)->Arg(2)->Setup(DoSetup)->Teardown(DoTeardown);
BENCHMARK(BM_VEC_THL)->Arg(10)->Arg(2)->Setup(DoSetup)->Teardown(DoTeardown);
