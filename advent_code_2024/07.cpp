//
// Created by ByteDance on 2024/12/9.
//

#include "07.h"

void Check(const std::vector<long long> &nums, long long pre_num, int cur_idx, int end_idx, bool &found) {
    auto cur = nums[cur_idx];
    int cal_decimal_digits = 0;
    while (cur) {
        cur /= 10;
        ++cal_decimal_digits;
    }

    if (cur_idx == end_idx) {
        if ((nums[0] == (pre_num + nums[cur_idx]))  || (nums[0] == (pre_num * nums[cur_idx])) || (nums[0] == (pre_num  * (std::pow(10, cal_decimal_digits)) + nums[cur_idx]))) {
            found = true;
        }
        return;
    }

    auto new_combined_num = pre_num * (std::pow(10, cal_decimal_digits)) + nums[cur_idx];
    Check(nums, new_combined_num, cur_idx + 1, end_idx, found);

    auto new_sum_pre = pre_num + nums[cur_idx];
    Check(nums, new_sum_pre, cur_idx + 1, end_idx, found);

    auto new_nul_pre = pre_num * nums[cur_idx];
    Check(nums, new_nul_pre, cur_idx + 1, end_idx, found);


}

long long GetTotalCalibrationResult() {
    std::ifstream file("/Users/bytedance/workspace/CodeSnippet/advent_code_2024/07.txt"); // 打开文件
    if (!file) {
        return -1;
    }

    std::string line;
    std::regex number("\\d+");
    std::smatch num;
    std::vector<std::vector<long long>> calibration;
    while (std::getline(file, line)) {
        std::vector<long long> this_line;
        while(std::regex_search(line, num, number)) {
            this_line.push_back(std::atoll(num.str().c_str()));
            line = num.suffix();
        }
        calibration.push_back(this_line);
    }
    long long sum = 0;
    for (int i = 0; i < calibration.size(); i++) {
        const auto &one_line_number = calibration[i];
        bool found = false;
        Check(one_line_number, one_line_number[1], 2, one_line_number.size() - 1, found);
        if (found) {
            sum += one_line_number[0];
        }
    }
    return sum;
}