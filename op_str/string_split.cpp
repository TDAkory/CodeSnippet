//
// Created by ByteDance on 2025/3/5.
//

#include "string_split.h"
#include <vector>
#include <sstream>
#include <ranges>
#if defined(__SSE__)
#include <immintrin.h>
#endif

std::vector<std::string> SplitByFind(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);
    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }
    if (start < str.size()) {
        tokens.push_back(str.substr(start));
    }
    return tokens;
}

std::vector<std::string> SplitByGetline(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream stream(str);
    std::string token;
    while (std::getline(stream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::vector<std::string> SplitC20(const std::string& s, char delimiter) {
    auto splitView = s | std::views::split(delimiter);
    std::vector<std::string> result;
    for (auto&& part : splitView) {
        result.emplace_back(part.begin(), part.end());
    }
    return result;
}

#if defined(__SSE__)
std::vector<std::string> SplitBySIMD(const std::string& input, char delimiter) {
    std::vector<std::string> result;
    const char* str = input.c_str();
    size_t len = input.length();
    size_t start = 0;

    // 初始化 SSE 寄存器，用于存储分隔符
    __m128i delimiterVec = _mm_set1_epi8(static_cast<char>(delimiter));

    for (size_t i = 0; i < len; i += 16) {
        // 处理剩余不足 16 个字符的情况
        if (i + 16 > len) {
            for (size_t j = i; j < len; ++j) {
                if (str[j] == delimiter) {
                    result.emplace_back(str + start, j - start);
                    start = j + 1;
                }
            }
            break;
        }

        // 加载 16 个字符到 SSE 寄存器
        __m128i charVec = _mm_loadu_si128(reinterpret_cast<const __m128i*>(str + i));
        // 比较 16 个字符是否等于分隔符
        __m128i cmpResult = _mm_cmpeq_epi8(charVec, delimiterVec);
        // 将比较结果转换为掩码，取每8bit的最高位，同时反序，即第16个byte在第1个bit上
        unsigned short mask = _mm_movemask_epi8(cmpResult);

        if (mask != 0) {
            // 一种选择，遍历mask
            // for (int k = 0; k < 16; ++k) {
            //     if ((mask & (1 << k)) != 0) {
            //         result.emplace_back(str + start, i + k - start);
            //         start = i + k + 1;
            //     }
            // }
            // 另一种选择，直接计算mask的尾缀零数量
            int past = 0;
            while (mask != 0) {
                auto pos = __builtin_ctz(mask);
                if (pos >= 16) {
                    break;
                }
                result.emplace_back(str + start, i + past + pos - start);
                start = i + past + pos + 1;
                past += (pos + 1);
                mask >>= (pos + 1);
            }
        }
    }

    // 处理最后一个子字符串
    if (start < len) {
        result.emplace_back(str + start, len - start);
    }

    return result;
}
#endif