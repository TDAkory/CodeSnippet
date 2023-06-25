//
// Created by zhaojieyi on 2023/6/25.
//

#include "scope_guard.h"
#include <iostream>
#include "gtest/gtest.h"

TEST(TestScopeGuard, TestOnExit) {
    int count = 64;
    {
        ScopeGuard guard([&]() {
            std::cout << "TestScopeGuard.TestOnExit onexit" << std::endl;
            count -= 12;
        });
    }
    std::cout << "TestScopeGuard.TestOnExit out of scope" << std::endl;
    EXPECT_EQ(64 - 12, count);
}