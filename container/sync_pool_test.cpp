//
// Created by zhaojieyi on 2023/6/3.
//

#include "sync_pool.h"
#include "gtest/gtest.h"

using namespace safe_container;

// TEST(TestSyncPool, TestNormalOps) {
//     SyncPool<int64_t> mypool;
//     EXPECT_EQ(SyncPool<int64_t>::default_cap, mypool.Cap());
//     EXPECT_NE(nullptr, mypool.stack_[0]);
//     auto *p = mypool.Get();
//     EXPECT_NE(nullptr, p);
//     mypool.Put(p);
//     EXPECT_EQ(1, mypool.CurIdx());
// }