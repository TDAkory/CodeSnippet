//
// Created by zhaojieyi on 2022/10/8.
//

#include "FileOps.h"
#include "sys/stat.h"
#include <fstream>

bool FileOps::Exist(const std::string &file) {
    struct stat s_buff;
    return (stat(file.c_str(), &s_buff) == 0);
}

bool FileOps::Remove(const std::string &file) {
    return (remove(file.c_str()) == 0);
}

void FileOps::WriteTo(const std::string &file, const std::vector<std::string> &&lines){
    if (Exist(file)) {
        (void) Remove(file);
    }

    std::ofstream ofs(file);
    for (const auto &l:lines) {
        ofs << l << std::endl;
    }
    return;
}