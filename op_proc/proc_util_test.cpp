//
// Created by zhaojieyi on 2023/3/30.
//

#include "proc_util.h"
#include <iostream>
#include "gtest/gtest.h"

TEST(TestProcUtil, TestProcMemUsage) {
    auto used = physical_memory_used_by_process();
    std::cout << used << std::endl;
    if (-1 == used) {
        std::cout << "No such file under OS" << std::endl;
    }
    EXPECT_NE(0, used);
}