//
// Created by zhaojieyi on 2023/6/27.
//

#include <iostream>
#include "bi_tree_printer.h"
#include "gtest/gtest.h"

using namespace safe_container;

struct TPoint {
    double x;
    double y;

    TPoint(double a, double b) : x(a), y(b) {}
    std::string ToString() { return std::to_string(x) + "," + std::to_string(y); }
};

TEST(TestBinarySearchTree, TestTreePrint) {
    TreeNode<TPoint> root(new TPoint(1, 2), nullptr, nullptr);
    root.left = new TreeNode<TPoint>(new TPoint(3, 4), nullptr, nullptr);
    root.right = new TreeNode<TPoint>(new TPoint(5, 6), nullptr, nullptr);

    root.right->left = new TreeNode<TPoint>(new TPoint(7, 8), nullptr, nullptr);

    PrintInfoNode *info = BuildPrintInfo<TreeNode<TPoint>>(&root);
    Printer p(info);
    std::cout << p << std::endl;
}