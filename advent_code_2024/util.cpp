//
// Created by ByteDance on 2024/12/3.
//

#include "util.h"
#include <iostream>

void PrintVec(const std::vector<int>& vec) {
    for (int i = 0; i < vec.size(); i++) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}