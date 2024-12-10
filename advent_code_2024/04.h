//
// Created by ByteDance on 2024/12/4.
//

#ifndef INC_04_H
#define INC_04_H

#include <fstream>

void DoCheckXMAS(int rows, int columns, int x, int y, const std::vector<std::vector<char>> & datas, int& count) {
    // upper-left
    if (x - 3 >= 0 && y -3 >= 0) {
        if (datas[x-1][y-1] == 'M' && datas[x-2][y-2] == 'A' && datas[x-3][y-3] == 'S') {
            count++;
        }
    }

    // upper
    if (y -3 >= 0) {
        if (datas[x][y-1] == 'M' && datas[x][y-2] == 'A' && datas[x][y-3] == 'S') {
            count++;
        }
    }

    // upper-right
    if (x + 3 <  columns && y -3 >= 0) {
        if (datas[x+1][y-1] == 'M' && datas[x+2][y-2] == 'A' && datas[x+3][y-3] == 'S') {
            count++;
        }
    }

    // right
    if (x + 3 <  columns) {
        if (datas[x+1][y] == 'M' && datas[x+2][y] == 'A' && datas[x+3][y] == 'S') {
            count++;
        }
    }

    // down-right
    if (x + 3 <  columns && y + 3 < rows) {
        if (datas[x+1][y+1] == 'M' && datas[x+2][y+2] == 'A' && datas[x+3][y+3] == 'S') {
            count++;
        }
    }

    // down
    if (y + 3 < rows) {
        if (datas[x][y+1] == 'M' && datas[x][y+2] == 'A' && datas[x][y+3] == 'S') {
            count++;
        }
    }

    // down left
    if (x - 3 >= 0 && y + 3 < rows) {
        if (datas[x-1][y+1] == 'M' && datas[x-2][y+2] == 'A' && datas[x-3][y+3] == 'S') {
            count++;
        }
    }

    // left
    if (x - 3 >= 0) {
        if (datas[x-1][y] == 'M' && datas[x-2][y] == 'A' && datas[x-3][y] == 'S') {
            count++;
        }
    }
}

void DoCheckX_MAS(int rows, int columns, int x, int y, const std::vector<std::vector<char>> & datas, int& count) {
    if (x - 1 >=0 && x + 1 < columns && y - 1 >= 0 && y + 1 < rows) {
        // upper-left to down right
        if (((datas[x-1][y-1] == 'M' && datas[x+1][y+1] == 'S') || (datas[x-1][y-1] == 'S' && datas[x+1][y+1] == 'M')) &&
            ((datas[x-1][y+1] == 'M' && datas[x+1][y-1] == 'S') || (datas[x-1][y+1] == 'S' && datas[x+1][y-1] == 'M'))) {
            count++;
        }
    }
}


int GetXMASCount() {
    std::ifstream file("/Users/bytedance/workspace/CodeSnippet/advent_code_2024/04.txt"); // 打开文件
    if (!file) {
        return -1;
    }

    std::vector<std::vector<char>> datas;
    std::string line;
    while (std::getline(file, line)) { // 逐行读取文件
        datas.emplace_back(std::vector<char>());
        auto &data = datas.back();
        for (const auto &c : line) {
            data.push_back(c);
        }
    }

    auto columns = datas.size();
    auto rows = datas[0].size();
    int count = 0;

    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < columns; y++) {
            // if (datas[x][y] == 'X') {
            //     DoCheckXMAS( rows,columns, x, y, datas, count);
            // }
            if (datas[x][y] == 'A') {
                DoCheckX_MAS(rows, columns, x, y, datas, count);
            }
        }
    }
    return count;
}

#endif //INC_04_H
