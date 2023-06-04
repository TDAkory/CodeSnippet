//
// Created by zhaojieyi on 2023/6/3.
//

#include "gtest/gtest.h"
#include "sync_pool.h"

using namespace safe_container;

TEST(TestSyncPool, TestNormalOps) {
    SyncPool<int64_t> mypool;
    auto *p = mypool.Get();
    EXPECT_NE(nullptr, p);
}