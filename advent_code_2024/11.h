//
// Created by ByteDance on 2024/12/11.
//

#ifndef INC_11_H
#define INC_11_H

#include <fstream>
#include <regex>
#include "util.h"

int CountDigits(long long n) {
    if (n == 0) return 1; // 特殊情况，0的位数为1
    return static_cast<int>(std::floor(std::log10(n)) + 1);
}

std::vector<long long> DoLoop(const std::vector<long long> &input) {
    std::vector<long long> output;
    for(const auto &n : input) {
        if (n ==0) {
            output.push_back(1);
            continue;
        }

        auto count = CountDigits(n);
        if (count % 2 == 0) {
            count /= 2;
            output.push_back(n / ((long)std::pow(10, count)));
            output.push_back(n % ((long)std::pow(10, count)));
            continue;
        }
        output.push_back(n * 2024);
    }
    return output;
}

struct Key {
    long long num;
    int loop_left;

    Key(long long n, int l) : num(n), loop_left(l) {}
};

bool operator<(const Key &a, const Key &b) {
    if (a.num == b.num) {
        return a.loop_left < b.loop_left;
    }
    return a.num < b.num;
}

using DP = std::map<Key, long long>;

long long DoDP(Key k, DP &dp) {
    if (dp.contains(k)) {
        return dp[k];
    }
    long long ret;
    auto count = CountDigits(k.num);
    if (k.loop_left == 0) {
        ret = 1;
    } else if (k.num == 0) {
        ret = DoDP({1, k.loop_left - 1}, dp);
    } else if (count % 2 == 0) {
        count /= 2;
        long long left = k.num / ((long long)std::pow(10, count));
        long long right = k.num % ((long long)std::pow(10, count));
        ret = DoDP({left, k.loop_left - 1}, dp) + DoDP({right, k.loop_left - 1}, dp);
    } else {
        ret = DoDP({k.num * 2024, k.loop_left - 1}, dp);
    }

    dp[k] = ret;
    return ret;
}

long long GetStoneCount() {
    std::ifstream file("/Users/bytedance/workspace/CodeSnippet/advent_code_2024/11.txt"); // 打开文件
    if (!file) {
        return -1;
    }
    std::string line;
    std::getline(file, line);
    std::regex re_number("\\d+");
    std::smatch match;
    std::vector<long long> numbers;
    while(std::regex_search(line, match, re_number)) {
        numbers.push_back(std::stoi(match.str()));
        line = match.suffix();
    }

    advent_of_code_2024_util::PrintVec(numbers);
    int loop_count = 75;
    // while(loop_count) {
    //     auto new_numbers = DoLoop(numbers);
    //     loop_count--;
    //     numbers.swap(new_numbers);
    //     std::cout << loop_count << ": " << numbers.size() << std::endl;
    // }
    long long sum = 0;
    DP dp;
    for(int i = 0; i < numbers.size(); i++) {
        sum += DoDP({numbers[i], loop_count}, dp);
    }
    return sum;
}
#endif //INC_11_H
