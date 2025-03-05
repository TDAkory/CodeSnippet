//
// Created by ByteDance on 2024/12/10.
//

#include "cmd_util.h"
#include <array>
#include <memory>

bool CMDUtil::RunCMDAndGetResult(const std::string &cmd, std::string &result) {
    if (cmd.empty()) {
        return false;
    }
    std::array<char, 128> buffer;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        return false;
    }
    while(fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return true;
}