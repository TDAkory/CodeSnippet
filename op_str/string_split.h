//
// Created by ByteDance on 2025/3/5.
//

#ifndef STRING_SPLIT_H
#define STRING_SPLIT_H

#include <string>
#include <vector>

std::vector<std::string> SplitByFind(const std::string &str, char delimiter);

std::vector<std::string> SplitByGetline(const std::string &str, char delimiter);

std::vector<std::string> SplitC20(const std::string &s, char delimiter);

#if defined(__SSE__)
std::vector<std::string> SplitBySIMD(const std::string &input, char delimiter);
#endif

#endif  // STRING_SPLIT_H
