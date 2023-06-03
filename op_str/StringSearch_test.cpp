//
// Created by zhaojieyi on 2022/10/8.
//

#include "StringSearch.h"
#include "gtest/gtest.h"

TEST(TestStringSearch, TestCaseInsensitive) {
    EXPECT_TRUE(ContainIgnoreCase("hello ppe", "Ppe"));
}