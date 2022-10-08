//
// Created by zhaojieyi on 2022/10/8.
//

#include "FileOps.h"
#include "gtest/gtest.h"

TEST(TestFileOps, TestWriteTo) {
    FileOps::WriteTo("tmp.txt", {"hello", "world"});
}