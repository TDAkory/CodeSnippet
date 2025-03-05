//
// Created by ByteDance on 2025/3/5.
//

#include "string_split.h"
#include "gtest/gtest.h"

TEST(TestStringSplit, TestSplitFunc) {
    std::string origin = "hellp^world^there^are^string^util_functions^for^doSomeSplit^123xcv8909()^cmodf912";
    char delimiter = '^';
    std::vector<std::string> result{"hellp", "world", "there", "are", "string", "util_functions", "for" ,"doSomeSplit", "123xcv8909()", "cmodf912"};

    {
        auto ret = SplitByFind(origin, delimiter);
        EXPECT_EQ(result.size(), ret.size());
        for (size_t i = 0; i < result.size(); i++) {
            EXPECT_EQ(result[i], ret[i]);
        }
    }
    {
        auto ret = SplitByGetline(origin, delimiter);
        EXPECT_EQ(result.size(), ret.size());
        for (size_t i = 0; i < result.size(); i++) {
            EXPECT_EQ(result[i], ret[i]);
        }
    }
    {
        auto ret = SplitC20(origin, delimiter);
        EXPECT_EQ(result.size(), ret.size());
        for (size_t i = 0; i < result.size(); i++) {
            EXPECT_EQ(result[i], ret[i]);
        }
    }
}