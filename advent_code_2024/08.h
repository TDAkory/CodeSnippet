//
// Created by ByteDance on 2024/12/9.
//

#ifndef INC_08_H
#define INC_08_H

#include <fstream>
#include <vector>
#include <map>
#include <set>

struct AntennaPos {
    int x;
    int y;

    AntennaPos(int i, int j) : x(i), y(j) {}
};

struct AntennaPosLess {
    bool operator()(const AntennaPos& lhs, const AntennaPos& rhs) const {
        if (lhs.x == rhs.x) {
            return lhs.y < rhs.y;
        }
        return lhs.x < rhs.x;
    }
};

inline std::vector<AntennaPos> GetAntiNode(const AntennaPos& l, const AntennaPos &r) {
    std::vector<AntennaPos> result;

    // l -- r -- anti
    result.emplace_back(r.x + (r.x - l.x), r.y + (r.y - l.y));

    // anti -- l -- r
    result.emplace_back(l.x + (l.x - r.x), l.y + (l.y - r.y));

    return result;
}

inline std::vector<AntennaPos> GetLinedAntiNode(const AntennaPos& l, const AntennaPos &r, const int row_max, const int col_max) {
    std::vector<AntennaPos> result;

    int delta_x = r.x - l.x;
    int delta_y = r.y - l.y;

    // l -- r -- anti
    int new_x = r.x + delta_x;
    int new_y = r.y + delta_y;
    while (new_x >=0 &&  new_x < row_max && new_y >= 0 && new_y < col_max) {
        result.emplace_back(new_x, new_y);
        new_x += delta_x;
        new_y += delta_y;
    }

    // anti -- l -- r
    new_x = l.x - delta_x;
    new_y = l.y - delta_y;
    while (new_x >= 0 && new_x < row_max && new_y >= 0 && new_y < col_max) {
        result.emplace_back(new_x, new_y);
        new_x -= delta_x;
        new_y -= delta_y;
    }
    return result;
}

inline bool CheckBoundary(const AntennaPos& pos, const int row_max, const int col_max) {
    if ( pos.x >= 0 && pos.x < row_max && pos.y >= 0 && pos.y < col_max ) {
        return true;
    }
    return false;
}

using AntennaMap = std::map<char, std::vector<AntennaPos>>;

int GetAntennaAntiNodeCount() {
    std::ifstream file("/Users/bytedance/workspace/CodeSnippet/advent_code_2024/08.txt"); // 打开文件
    if (!file) {
        return -1;
    }

    AntennaMap a_map;
    std::string line;
    int i = 0;
    while (std::getline(file, line)) {
        for(int j = 0; j < line.size(); j++) {
            if (line[j] != '.') {
                if (!a_map.contains(line[j])) {
                    a_map[line[j]] = std::vector<AntennaPos>();
                }
                a_map[line[j]].emplace_back(i, j);
            }
        }
        i++;
    }

    int row_max = i;
    int col_max = line.size();

    int sum = 0;
    std::set<AntennaPos, AntennaPosLess, std::allocator<AntennaPos>> visited;
    for (const auto it : a_map) {
        auto size = it.second.size();
        for (int i = 0; i < size; i++) {
            for (int j = i + 1; j < size; j++) {
                auto antinodes = GetLinedAntiNode(it.second[i], it.second[j], row_max, col_max);
                if (!visited.contains(it.second[i])) {
                    sum++;
                    visited.insert(it.second[i]);
                }
                if (!visited.contains(it.second[j])) {
                    sum++;
                    visited.insert(it.second[j]);
                }
                for (const auto node : antinodes) {
                    if (!visited.contains(node)) {
                        sum++;
                        visited.insert(node);
                    }
                }
            }
        }
    }
    return sum;
}

#endif //INC_08_H
