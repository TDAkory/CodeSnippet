//
// Created by zhaojieyi on 2023/6/27.
//
#include "bi_tree_printer.h"

namespace safe_container {

Printer::Printer(PrintInfoNode *root) : root(root) {
    setLayerLink();
    initPosInfo();
    agjustPosInfo();
}

Printer::~Printer() {
    if (this->root) {
        delete this->root;
        this->root = nullptr;
    }
}

void Printer::setLayerLink() {
    if (this->root == nullptr)
        return;
    this->layerHead.clear();
    std::queue<PrintInfoNode *> Q;
    Q.push(this->root);
    while (!Q.empty()) {
        this->layerHead.push_back(Q.front());
        int size = Q.size();
        while (size--) {
            PrintInfoNode *cur = Q.front();
            Q.pop();
            if (size > 0)
                cur->next = Q.front();
            if (cur->left)
                Q.push(cur->left);
            if (cur->right)
                Q.push(cur->right);
        }
    }
}

void Printer::initPosInfo() {
    if (this->root == nullptr || this->layerHead.empty())
        return;
    this->root->pos = 0;
    for (size_t i = 1; i < this->layerHead.size(); i++) {
        PrintInfoNode *cur = this->layerHead[i];
        while (cur) {
            if (cur == cur->parent->left) {
                cur->pos = cur->parent->pos - 2 - cur->middle;
            } else {
                cur->pos = cur->parent->pos + cur->parent->last + 2 - cur->middle;
            }
            cur = cur->next;
        }
    }
}

void Printer::agjustPosInfo() {
    if (this->root == nullptr)
        return;
    for (int i = this->layerHead.size() - 2; i >= 0; i--) {
        PrintInfoNode *cur = this->layerHead[i];
        while (cur) {
            int cross = this->calcCrossLen(cur);
            if (cross > 0) {
                int left = cross / 2;
                this->movePos(cur->left, -left);
                this->movePos(cur->right, cross - left);
            }
            cur = cur->next;
        }
    }
    int minPos = 0;
    for (auto node : this->layerHead) {
        minPos = node->pos < minPos ? node->pos : minPos;
    }
    if (minPos < 0) {
        this->movePos(this->root, -minPos);
    }
}

int Printer::calcCrossLen(PrintInfoNode *node) {
    if (node == nullptr || node->left == nullptr || node->right == nullptr)
        return 0;
    std::vector<int> leftRight;
    std::vector<int> rightLeft;
    std::queue<PrintInfoNode *> Q;
    Q.push(node->left);
    while (!Q.empty()) {
        int size = Q.size();
        while (size > 1) {
            PrintInfoNode *cur = Q.front();
            Q.pop();
            if (cur->left)
                Q.push(cur->left);
            if (cur->right)
                Q.push(cur->right);
            size--;
        }
        PrintInfoNode *cur = Q.front();
        Q.pop();
        if (cur->left)
            Q.push(cur->left);
        if (cur->right)
            Q.push(cur->right);
        if (cur->right) {
            leftRight.push_back(cur->right->pos + cur->right->middle);
        } else {
            leftRight.push_back(cur->pos + cur->last);
        }
    }
    Q.push(node->right);
    while (!Q.empty() && rightLeft.size() < leftRight.size()) {
        PrintInfoNode *cur = Q.front();
        Q.pop();
        int size = Q.size();
        if (cur->left)
            Q.push(cur->left);
        if (cur->right)
            Q.push(cur->right);
        while (size--) {
            PrintInfoNode *cur = Q.front();
            Q.pop();
            if (cur->left)
                Q.push(cur->left);
            if (cur->right)
                Q.push(cur->right);
        }
        if (cur->left) {
            rightLeft.push_back(cur->left->pos + cur->left->middle);
        } else {
            rightLeft.push_back(cur->pos);
        }
    }
    const int minSpace = 1;
    int cross = 0;
    for (int i = 0; i < rightLeft.size(); i++) {
        int tmp = leftRight[i] + minSpace - rightLeft[i] + 1;
        cross = tmp > cross ? tmp : cross;
    }
    return cross;
}

void Printer::movePos(PrintInfoNode *node, int len) {
    if (node->left)
        this->movePos(node->left, len);
    if (node->right)
        this->movePos(node->right, len);
    node->pos += len;
}

std::ostream &operator<<(std::ostream &os, Printer &tree) {
    if (tree.root == nullptr)
        return os;
    std::stringstream ss;

    for (auto cur : tree.layerHead) {
        int idx = 0;
        ss.str("");
        while (cur) {
            if (cur->left) {
                int limit = cur->left->pos + cur->left->middle;
                while (idx < limit) {
                    os << ' ';
                    ss << ' ';
                    idx++;
                }
                os << "┌";
                ss << "│";
                idx++;
                limit = cur->pos;
                while (idx < limit) {
                    os << "─";
                    ss << ' ';
                    idx++;
                }
            } else {
                int limit = cur->pos;
                while (idx < limit) {
                    os << ' ';
                    ss << ' ';
                    idx++;
                }
            }
            os << cur->content;
            int limit = cur->pos + cur->last + 1;
            while (idx < limit) {
                ss << ' ';
                idx++;
            }
            if (cur->right) {
                int limit = cur->right->pos + cur->right->middle;
                while (idx < limit) {
                    os << "─";
                    ss << ' ';
                    idx++;
                }
                os << "┐";
                ss << "│";
                idx++;
            }
            cur = cur->next;
        }
        os << '\n' << ss.str() << '\n';
    }
    return os;
}

}  // namespace safe_container