//
// Created by zhaojieyi on 2022/10/8.
//

#ifndef CODESNIPPET_FILEOPS_H
#define CODESNIPPET_FILEOPS_H]

#include <string>
#include <vector>

class FileOps {
public:
    static bool Exist(const std::string &file);

    static bool Remove(const std::string &file);

    static void WriteTo(const std::string &file, const std::vector<std::string> &&lines);

};

#endif //CODESNIPPET_FILEOPS_H
