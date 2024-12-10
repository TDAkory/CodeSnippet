//
// Created by ByteDance on 2024/12/5.
//

#ifndef INC_05_H
#define INC_05_H

#include <fstream>
#include <map>
#include <regex>
#include <iostream>

int GetCorrectLineMiddleSum() {
    std::ifstream file("/Users/bytedance/workspace/CodeSnippet/advent_code_2024/05.txt"); // 打开文件
    if (!file) {
        return -1;
    }

    std::map<int, std::vector<int>> rules;
    std::vector<std::vector<int>> lines;

    std::regex rule_pattern("(\\d{2})\\|(\\d{2})");
    std::regex pages_pattern("(\\d{2},)+\\d{2}");
    std::regex page_pattern("\\d{2}");

    std::string line;
    while (std::getline(file, line)) { // 逐行读取文件
        std::smatch match;
        if (std::regex_search(line, match,rule_pattern)) {
            auto pre = atoi(match[1].str().c_str());
            auto after = atoi(match[2].str().c_str());
            if (!rules.contains(after)) {
                rules[after] = std::vector<int>();
            }
            rules[after].push_back(pre);
        }

        if (std::regex_match(line, pages_pattern)) {
            lines.push_back(std::vector<int>());
            auto &this_line = lines.back();
            while(!line.empty()) {
                std::regex_search(line, match, page_pattern);
                auto p = atoi(match.str().c_str());
                this_line.push_back(p);
                line = match.suffix();
            }
        }
    }

    int sum = 0;
    for (auto &numbers : lines) {
        int size = numbers.size();
        bool legal = true;
        for (int i = 0; i < size; i++) {
            if (!rules.contains(numbers[i])) {
                continue;
            }
            auto &pres = rules[numbers[i]];
            for (int j = i+1; j < size; j++) {

                if (std::find(pres.begin(), pres.end(), numbers[j]) != pres.end()) {
                    legal =false;
                    break;
                }
            }
            if (!legal) {
                break;
            }
        }

        if (!legal) {
            // Do Correct, 反向冒泡
            for(int i = 0; i < size; i++) {
                for (int j = 0; j < size - i - 1; j++) {
                    auto l = numbers[j];
                    if (!rules.contains(l)) {
                        continue;
                    }
                    auto &pres = rules[l];
                    if (std::find(pres.begin(), pres.end(), numbers[j+1]) != pres.end()) {
                        auto tmp = numbers[j+1];
                        numbers[j+1] = numbers[j];
                        numbers[j] = tmp;
                    }

                }
            }
            sum += numbers[size/2];
        }
    }
    return sum;
}

#endif //INC_05_H
