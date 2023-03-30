//
// Created by zhaojieyi on 2023/3/30.
//

#include "proc_util.h"
#include "gtest/gtest.h"
#include <iostream>

TEST(TestProcUtil, TestProcMemUsage) {
  std::cout << physical_memory_used_by_process() << std::endl;
  EXPECT_NE(0, physical_memory_used_by_process());
}