//
// Created by ByteDance on 2024/12/10.
//

#include "cmd_util.h"
#include "gtest/gtest.h"

TEST(TestCMDUtil, TestRunCMD) {
    std::string result;
    EXPECT_TRUE(CMDUtil::RunCMDAndGetResult("ls -l", result));
    EXPECT_NE(0, result.size());
    std::cout << result << std::endl;
}