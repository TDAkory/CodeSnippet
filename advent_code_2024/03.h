//
// Created by ByteDance on 2024/12/3.
//

#ifndef INC_03_H
#define INC_03_H

#include <regex>

int GetMulSum() {
    std::ifstream file("/Users/bytedance/workspace/CodeSnippet/advent_code_2024/03.txt"); // 打开文件
    if (!file) {
        return -1;
    }

    int sum = 0;
    std::regex mul_pattern("mul\\((\\d{1,3})\\,(\\d{1,3})\\)"); // 编译正则表达式
    std::regex do_pattern("do\\(\\)");
    std::regex dont_pattern("don't\\(\\)");
    std::string line;

    bool enable = true;
    while (std::getline(file, line)) { // 逐行读取文件
        std::smatch mul_match;
        int p_size_mul = INT_MAX;
        std::smatch do_match;
        int p_size_do = INT_MAX;
        std::smatch dont_match;
        int p_size_dont = INT_MAX;
        int p_size_min = INT_MAX;

        while (!line.empty()) {
            if (std::regex_search(line, mul_match, mul_pattern)) {
                p_size_mul = mul_match.prefix().length();
            }
            if (std::regex_search(line, do_match, do_pattern)) {
                p_size_do = do_match.prefix().length();
            }
            if (std::regex_search(line, dont_match, dont_pattern)) {
                p_size_dont = dont_match.prefix().length();
            }

            if (p_size_mul == INT_MAX && p_size_do == INT_MAX && p_size_dont == INT_MAX) {
                break;
            }

            p_size_min = std::min(p_size_mul, p_size_do);
            p_size_min = std::min(p_size_min, p_size_dont);

            if (p_size_min == p_size_mul) {
                std::cout << mul_match.str(0) << std::endl;
                if (enable) {
                    auto l = atoi(mul_match.str(1).c_str());
                    auto r = atoi(mul_match.str(2).c_str());
                    sum += l * r;
                }
                line = mul_match.suffix();
            } else if (p_size_min == p_size_do ) {
                std::cout << do_match.str(0) << std::endl;
                enable = true;
                line = do_match.suffix();
            } else if (p_size_min == p_size_dont ) {
                std::cout << dont_match.str(0) << std::endl;
                enable = false;
                line = dont_match.suffix();
            }
            p_size_mul = p_size_do = p_size_dont = INT_MAX;
        }
    }
    return sum;
}

#endif //INC_03_H
