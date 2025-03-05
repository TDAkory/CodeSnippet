//
// Created by zhaojieyi on 2022/10/8.
//

#include "string_search.h"
#include <algorithm>

bool ContainIgnoreCase(const std::string &cont, const std::string &s) {
    return (std::search(cont.begin(), cont.end(), s.begin(), s.end(),
                [](char c1, char c2) { return std::toupper(c1) == std::toupper(c2); }) != cont.end());
}