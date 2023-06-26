//
// Created by zhaojieyi on 2023/6/26.
//

#ifndef CODESNIPPET_BINARY_TREE_H
#define CODESNIPPET_BINARY_TREE_H

#include <string>

namespace safe_container {

template <typename T>
struct TreeNode {
    T *data;
    TreeNode *left;
    TreeNode *right;

    std::string ToString();
};

template <typename T>
int32_t GetBinaryTreeWidth(T *root) {
    if (!root) {
        return 0;
    }
}

}  // namespace safe_container

#endif  // CODESNIPPET_BINARY_TREE_H

#pragma once

#include <string>
#include <vector>

class Printer {
 private:
    class InfoNode;

 private:
    InfoNode *root = nullptr;
    std::vector<InfoNode *> layerHead;  // 用于层序遍历，每一层的第一个结点

 public:
    Printer() = default;
    virtual ~Printer();

    friend std::ostream &operator<<(std::ostream &os, Printer &tree);

 protected:
    virtual void *getRoot() const = 0;
    virtual void *getLeft(void *node) const = 0;
    virtual void *getRight(void *node) const = 0;
    virtual std::string getString(void *node) const = 0;

 private:
    void initTreeStructure();  // 初始化树结构，并填充结点信息（除 pos 之外）
    void setLayerLink();       // 设置层内链接，填充 layerHead
    void initPosInfo();        // 初始化位置信息
    void agjustPosInfo();      // 解决冲突

    InfoNode *initTreeStructure(void *node, InfoNode *parent);  // 根据传入信息生成树，返回根节点（递归）
    int calcCrossLen(InfoNode *node);       // 计算所给节点的两棵子树的最大冲突长度
    void movePos(InfoNode *node, int len);  // 移动整棵树
};