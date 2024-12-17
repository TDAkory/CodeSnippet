//
// Created by ByteDance on 2024/12/9.
//

#ifndef INC_09_H
#define INC_09_H

#include <fstream>
#include <vector>

struct Block {
    int start_idx{0};
    int cap{0};

    Block() = default;
    Block(int i, int j) : start_idx(i), cap(j) {}
};

struct BlockCmp {
    bool operator()(const Block& lhs, const Block& rhs) const {
        return lhs.start_idx < rhs.start_idx;
    }
};

inline bool FindAvaliableEmptyChunk(const std::vector<int> &blocks, const Block & file, int &start_idx) {
    Block empty;
    for (int i = 0; i < blocks.size(); i++) {
        if (blocks[i] != INT_MIN) {
            continue;
        }
        empty.start_idx = i;
        while (blocks[i] == INT_MIN) {
            empty.cap++;
            i++;
        }

        if (empty.cap >= file.cap && empty.start_idx < file.start_idx) {
            start_idx = empty.start_idx;
            return true;
        }
        empty.cap = 0;
    }
    return false;
}

using Blocks = std::vector<Block>; // start_idx, cap

long GetCompactCheckSum() {
    std::ifstream file("/Users/bytedance/workspace/CodeSnippet/advent_code_2024/09.txt"); // 打开文件
    if (!file) {
        return -1;
    }

    std::string line;
    bool block = true;
    int block_id = 0;
    std::vector<int> blocks;
    Blocks file_blocks;
    while (std::getline(file, line)) {
        for (const auto c : line) {
            long num = c - '0';
            if (block) {
                file_blocks.emplace_back(blocks.size(), num);
                while(num > 0) {
                    blocks.push_back(block_id);
                    num--;
                }
                block = !block;
                block_id++;
            } else {
                while (num > 0) {
                    blocks.push_back(INT_MIN);
                    num--;
                }
                block = !block;
            }
        }
    }

   for (int i = file_blocks.size() - 1; i >= 0; i--) {
        auto &file_block = file_blocks[i];
        int start_idx = -1;
        if (FindAvaliableEmptyChunk(blocks, file_block, start_idx)) {
            auto copy_times = file_block.cap;
            auto file_start = file_block.start_idx;
            while (copy_times) {
                blocks[start_idx] = blocks[file_start];
                blocks[file_start] = INT_MIN;
                start_idx++;
                file_start++;
                copy_times--;
            }
        }

   }

    long long sum = 0;
    for (int i = 0; i < blocks.size(); i++) {
        if (blocks[i] != INT_MIN) {
            sum += blocks[i] * i;
        }
    }
    return sum;
}

#endif //INC_09_H
