//
// Created by ByteDance on 2024/12/11.
//

#ifndef INC_10_H
#define INC_10_H

#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <cassert>

struct Pos {
    int x;
    int y;

    Pos(int _x, int _y) : x(_x), y(_y) {}
};

struct PosCmp {
    bool operator()(const Pos& p1, const Pos& p2) {
        if (p1.x == p2.x) {
            return p1.y < p2.y;
        }
        return p1.x < p2.x;
    }
};

struct PosEqual {
    bool operator()(const Pos& p1, const Pos& p2) {
        return p1.x == p2.x && p1.y == p2.y;
    }
};

using Path = std::vector<Pos>;

struct PathCmp {
    bool operator()(const Path& p1, const Path& p2) {
        assert(10 == p1.size());
        assert(10 == p2.size());
        for (auto i = 0; i < p1.size(); i++) {
            if (!PosEqual()(p1[i], p2[i])) {
                return PosCmp()(p1[i], p2[i]);
            }
        }
        return false;
    }
};

using RoadMap = std::vector<std::vector<int>>;
using Visited = std::set<Path, PathCmp>;

void FindPathCount(Pos p, int cur_height, Path &path,const RoadMap &maps, Visited &visited) {
    path.emplace_back(p.x, p.y);
    if (maps[p.x][p.y] == 9 && visited.find(path) == visited.end()) {
        visited.insert(path);
        return;
    }

    auto next_height = cur_height + 1;

    if (((p.x - 1) >= 0) && (maps[p.x - 1][p.y] == next_height)) {
        FindPathCount({p.x - 1, p.y}, next_height, path, maps, visited);
        path.pop_back();
    }

    if (((p.x + 1) < maps.size()) && (maps[p.x + 1][p.y] == next_height)) {
       FindPathCount({p.x + 1, p.y}, next_height, path, maps, visited);
        path.pop_back();
    }

    if (((p.y - 1) >= 0) && (maps[p.x][p.y - 1] == next_height)) {
        FindPathCount({p.x, p.y - 1}, next_height,path, maps, visited);
        path.pop_back();
    }

    if (((p.y + 1) < maps[0].size()) && (maps[p.x][p.y + 1] == next_height)) {
        FindPathCount({p.x, p.y + 1}, next_height, path, maps, visited);
        path.pop_back();
    }
}

int GetAllPathCount() {
    std::ifstream file("/Users/bytedance/workspace/CodeSnippet/advent_code_2024/10.txt"); // 打开文件
    if (!file) {
        return -1;
    }
    RoadMap maps;
    std::vector<Pos> starters;

    std::string line;
    int i = 0;
    while (std::getline(file, line)) {
        std::vector<int> map_line;
        for (int j = 0; j < line.size(); j++) {
            auto num = line[j] - '0';
            map_line.push_back(num);
            if (num == 0) {
                starters.emplace_back(i,j);
            }
        }
        maps.push_back(map_line);
        i++;
    }

    int sum = 0;
    for(const auto& pos: starters) {
        Visited visited;
        Path path;
        FindPathCount(pos, 0, path, maps, visited);
        sum += visited.size();
    }
    return sum;
}

#endif //INC_10_H
