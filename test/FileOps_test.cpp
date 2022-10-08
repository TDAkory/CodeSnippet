//
// Created by zhaojieyi on 2022/10/8.
//

#include "FileOps.h"
#include "gtest/gtest.h"

TEST(TestFileOps, TestWriteTo) {
    FileOps::OverWriteTo("tmp.txt", {"hello", "world"});
}