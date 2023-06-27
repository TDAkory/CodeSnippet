//
// Created by zhaojieyi on 2023/6/26.
//

#ifndef CODESNIPPET_BI_TREE_PRINTER_H
#define CODESNIPPET_BI_TREE_PRINTER_H

#include <queue>
#include <sstream>
#include <string>
#include <tuple>

namespace safe_container {

template <typename T>
struct TreeNode {
    T *data;
    TreeNode *left;
    TreeNode *right;

    TreeNode(T *d, TreeNode<T> *l, TreeNode<T> *r) : data(d), left(l), right(r) {}
    std::string ToString();
};

template <typename T>
std::string TreeNode<T>::ToString() {
    return data->ToString();
}

struct PrintInfoNode {
    std::string content;
    PrintInfoNode *parent;
    PrintInfoNode *left = nullptr;
    PrintInfoNode *right = nullptr;
    PrintInfoNode *next = nullptr;  // 同一层的下一个节点
    int pos = 0;                    // 结点起始位置
    int middle = 0;                 // 结点中心距起始位置的距离
    int last = 0;                   // 结点末尾距起始位置的距离

    PrintInfoNode(const std::string &s, PrintInfoNode *p) : content(s), parent(p) {}
    ~PrintInfoNode() {
        if (this->left)
            delete this->left;
        if (this->right)
            delete this->right;
        this->left = nullptr;
        this->right = nullptr;
    }
};

template <typename T>
PrintInfoNode *BuildPrintInfo(T *node) {
    if (node == nullptr)
        return nullptr;
    auto *root = new PrintInfoNode(node->ToString(), nullptr);
    root->middle = root->content.size() / 2;
    root->last = root->content.size() - 1;

    std::queue<std::tuple<PrintInfoNode *, T *, int>> q;
    if (node->left)
        q.push(std::make_tuple(root, node->left, 0));
    if (node->right)
        q.push(std::make_tuple(root, node->right, 1));
    while (!q.empty()) {
        auto t = q.front();
        auto *parent = std::get<0>(t);
        auto *cur_node = std::get<1>(t);
        bool left = std::get<2>(t) == 0;

        auto *cur = new PrintInfoNode(cur_node->ToString(), parent);
        cur->middle = cur->content.size() / 2;
        cur->last = cur->content.size() - 1;

        if (left) {
            parent->left = cur;
        } else {
            parent->right = cur;
        }

        if (cur_node->left)
            q.push(std::make_tuple(cur, cur_node->left, 0));
        if (cur_node->right)
            q.push(std::make_tuple(cur, cur_node->right, 1));
        q.pop();
    }
    return root;
}

class Printer {
 private:
    PrintInfoNode *root{nullptr};
    std::vector<PrintInfoNode *> layerHead;  // 用于层序遍历，每一层的第一个结点

 public:
    explicit Printer(PrintInfoNode *root);
    virtual ~Printer();

    friend std::ostream &operator<<(std::ostream &os, Printer &tree);

 private:
    void setLayerLink();   // 设置层内链接，填充 layerHead
    void initPosInfo();    // 初始化位置信息
    void agjustPosInfo();  // 解决冲突

    // 根据传入信息生成树，返回根节点（递归）
    int calcCrossLen(PrintInfoNode *node);       // 计算所给节点的两棵子树的最大冲突长度
    void movePos(PrintInfoNode *node, int len);  // 移动整棵树
};

}  // namespace safe_container

#endif  // CODESNIPPET_BI_TREE_PRINTER_H
