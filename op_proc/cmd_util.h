//
// Created by ByteDance on 2024/12/10.
//

#ifndef CMD_UTIL_H
#define CMD_UTIL_H

#include <string>

class CMDUtil {
    public:
    static bool RunCMDAndGetResult(const std::string &cmd, std::string &result);
};



#endif //CMD_UTIL_H
