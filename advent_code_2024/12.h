//
// Created by ByteDance on 2024/12/12.
//

#ifndef INC_12_H
#define INC_12_H

#include <fstream>
#include <vector>
#include "util.h"

using namespace advent_of_code_2024_util;

void DetectOneRegion(const std::vector<std::vector<char>>& map, int i, int j, const char c, std::vector<std::vector<bool>>& visited, int &area, int&edge) {
    area++;
    visited[i][j] = true;

    // up
    if (i -1 >=0 && map[i-1][j] == c && !visited[i - 1][j]) {
        DetectOneRegion(map, i-1, j, c, visited, area, edge);
    } else if (i - 1 < 0 || map[i-1][j] !=c) {
        edge++;
    }

    // down
    if (i + 1 < map.size() && map[i+1][j] == c  && !visited[i + 1][j]) {
        DetectOneRegion(map, i+1, j, c, visited, area, edge);
    }else if (i + 1 >= map.size() || map[i+1][j] != c) {
        edge++;
    }

    // left
    if (j -1 >=0 && map[i][j - 1] == c && !visited[i][j - 1]) {
        DetectOneRegion(map, i, j - 1, c, visited, area, edge);
    }else if (j - 1 < 0 || map[i][j - 1] != c) {
        edge++;
    }

    // right
    if (j + 1 < map[0].size() && map[i][j + 1] == c  && !visited[i][j + 1]) {
        DetectOneRegion(map, i, j + 1, c, visited, area, edge);
    }else if (j + 1 >= map.size() || map[i][j + 1] != c) {
        edge++;
    }
}

int CalPerimeterBFS(const std::vector<std::vector<char>>& map, std::vector<std::vector<bool>>& visited) {
    int sum = 0;
    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[0].size(); ++j) {
            if (visited[i][j]) {
                continue;
            }
            int area = 0;
            std::queue<Point> q;
            std::map<Point, std::set<Point>> m;
            q.push(Point(i, j));
            while (!q.empty()) {
                int x = q.front().x;
                int y = q.front().y;
                q.pop();
                if (visited[x][y]) {
                    continue;
                }
                visited[x][y] = true;
                area++;
                for (const auto &d : g_directions) {
                    auto new_x = x + d.x, new_y = y + d.y;
                    if (0 <= new_x && new_x < map.size() && 0 <= new_y && new_y < map[0].size() && map[new_x][new_y] == map[x][y]) {
                        q.push(Point(new_x , new_y ));
                    } else {
                        if (!m.contains(d)) {
                            m[d] = std::set<Point>();
                        }
                        m[d].insert(Point(x, y));
                    }
                }
            }

            int sides = 0;
            for (const auto &it : m) {
                std::set<Point> visited_perim;
                for (auto &p : it.second) {
                    if (visited_perim.find(p) == visited_perim.end()) {
                        sides++;
                        std::queue<Point> side_q;
                        side_q.push(p);
                        while(!side_q.empty()) {
                            Point in_p = side_q.front();
                            side_q.pop();
                            if (visited_perim.find(in_p) != visited_perim.end()) {
                                continue;
                            }
                            visited_perim.insert(in_p);
                            for (const auto &d : g_directions) {
                                auto new_x = in_p.x + d.x, new_y = in_p.y + d.y;
                                if (it.second.contains(Point(new_x, new_y))) {
                                    side_q.push(Point(new_x, new_y));
                                }
                            }
                        }
                    }
                }
            }
            sum += sides * area;
        }
    }
    return sum;
}

int GetFenceCost() {
    std::ifstream file("/Users/bytedance/workspace/CodeSnippet/advent_code_2024/12.txt"); // 打开文件
    if (!file) {
        return -1;
    }

    std::vector<std::vector<char>> garden_map;
    std::vector<std::vector<bool>> visited;

    std::string line;

    while (std::getline(file, line)) {
        std::vector<char> one_line;
        for (int i = 0; i < line.size(); i++) {
            one_line.push_back(line[i]);
        }
        std::vector<bool> line_visited(one_line.size(), false);

        garden_map.push_back(one_line);
        visited.push_back(line_visited);
    }


    // Actually DFS
    // int total_cost = 0;
    // for (int i = 0; i < garden_map.size(); i++) {
    //     for (int j = 0; j < garden_map[i].size(); j++) {
    //         if (visited[i][j]) {
    //             continue;
    //         }
    //         int area = 0;
    //         int edge = 0;
    //         DetectOneRegion(garden_map, i, j, garden_map[i][j], visited, area, edge);
    //         total_cost += (area * edge);
    //     }
    // }
    return CalPerimeterBFS(garden_map, visited);
}

#endif //INC_12_H
