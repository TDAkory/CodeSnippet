//
// Created by ByteDance on 2024/12/9.
//

#ifndef INC_09_H
#define INC_09_H

#include <fstream>
#include <vector>

struct Block {
    int start_idx;
    int cap;

    Block(int i, int j) : start_idx(i), cap(j) {}
};

struct BlockCmp {
    bool operator()(const Block& lhs, const Block& rhs) const {
        return lhs.start_idx < rhs.start_idx;
    }
};

using Blocks = std::vector<Block>; // start_idx, cap

long GetCompactCheckSum() {
    std::ifstream file("/Users/bytedance/workspace/CodeSnippet/advent_code_2024/tmp.txt"); // 打开文件
    if (!file) {
        return -1;
    }

    std::string line;
    bool block = true;
    int block_id = 0;
    std::vector<int> blocks;
    Blocks empty_blocks;
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
                if (num > 0) {
                    empty_blocks.emplace_back(blocks.size(), num);
                }
                while (num > 0) {
                    blocks.push_back(INT_MIN);
                    num--;
                }
                block = !block;
            }
        }
    }

    std::sort(empty_blocks.begin(), empty_blocks.end(), BlockCmp());

   for (int i = file_blocks.size() - 1; i >= 0; i--) {
        auto &file_block = file_blocks[i];
        for (auto it = empty_blocks.begin(); it != empty_blocks.end(); it++) {
            if (it->cap >= file_block.cap) {
                auto copy_times = file_block.cap;
                auto dest_idx = it->start_idx;
                auto src_idx = file_block.start_idx;
                while(copy_times) {
                    blocks[dest_idx] = blocks[src_idx];
                    blocks[src_idx] = INT_MIN;
                    dest_idx++;
                    src_idx++;
                    copy_times--;
                }
                auto left = it->cap - file_block.cap;
                auto new_empty_start = it->start_idx + file_block.cap;
                empty_blocks.erase(it);
                if (left) {
                    empty_blocks.emplace_back(new_empty_start, left);
                }
                std::sort(empty_blocks.begin(), empty_blocks.end(), BlockCmp());
                break;
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
