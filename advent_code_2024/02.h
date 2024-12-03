//
// Created by ByteDance on 2024/12/2.
//

#ifndef INC_02_H
#define INC_02_H

#include <fstream>
#include <sstream>
#include <vector>
#include "util.h"

bool DoCheck(const std::vector<int> & tokens) {
    bool increasing = tokens[0] < tokens[1];
    bool pass = true;
    for (int i = 0; i < tokens.size() - 1; i++) {
        if (tokens[i] == tokens[i+1]) {
            pass = false;
            break;
        }
        if (increasing) {
            if (tokens[i] >= tokens[i+1]) {
                pass = false;
                break;
            }
            auto gap = std::abs(tokens[i] - tokens[i+1]);
            if (!(gap >= 1 && gap <= 3)) {
                pass = false;
                break;
            }
        } else {
            if (tokens[i] <= tokens[i+1]) {
                pass = false;
                break;
            }
            auto gap = std::abs(tokens[i] - tokens[i+1]);
            if (!(gap >= 1 && gap <= 3)) {
                pass = false;
                break;
            }
        }
    }
    return pass;
}

int GetSafetyLine() {
    std::ifstream file("/Users/bytedance/workspace/CodeSnippet/advent_code_2024/02.txt"); // 打开文件
    if (!file) {
        return -1;
    }

    int sum = 0;

    std::string line;
    std::vector<int> tokens;

    while (std::getline(file, line)) { // 逐行读取文件
        tokens.clear();
        std::istringstream linestream(line);
        std::string token;
        while(std::getline(linestream, token, ' ')) {
            tokens.push_back(atoi(token.c_str()));
        }

        if (DoCheck(tokens)) {
            sum++;
        } else {
            for (int i = 0; i < tokens.size(); i++) {
                auto tmp = tokens;
                tmp.erase(tmp.begin() + i);
                if (DoCheck(tmp)) {
                    sum++;
                    break;
                }
            }
        }
    }
    return sum;
}

#endif //INC_02_H
