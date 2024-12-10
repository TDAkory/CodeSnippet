//
// Created by ByteDance on 2024/12/6.
//

#ifndef INC_06_H
#define INC_06_H

#include <fstream>
#include <vector>

int GetPatrolPositionCount() {
    std::ifstream file("/Users/bytedance/workspace/CodeSnippet/advent_code_2024/06.txt"); // 打开文件
    if (!file) {
        return -1;
    }
    std::string line;
    std::vector<std::vector<char>> maps;
    int direction = -1;
    constexpr int left = 1;
    constexpr int up = 2;
    constexpr int right =3;
    constexpr int down = 4;
    int x = -1, y = -1;
    int i = 0;
    while(std::getline(file, line)) {
        std::vector<char> this_line;
        for(int j = 0; j < line.size(); j++) {
            if (line[j] == '^') {
                x = i;
                y = j;
                direction = up;
                this_line.push_back('.');
            } else {
                this_line.push_back(line[j]);
            }
        }
        maps.push_back(this_line);
        i++;
    }

    int row = maps.size();
    int col = maps[0].size();
    int sum = 1;
    maps[x][y] = 'x';
    while (true) {
        if (direction == up) {
            if (maps[x - 1][y] != '#') {
                x -= 1;
                if (maps[x][y] != 'x') {
                    sum++;
                }
            } else {
                direction = right;
            }
        } else if (direction == right) {
            if (maps[x][y+1] != '#') {
                y += 1;
                if (maps[x][y] != 'x') {
                    sum++;
                }
            } else {
                direction = down;
            }
        } else if (direction == down) {
            if (maps[x + 1][y] != '#') {
                x += 1;
                if (maps[x][y] != 'x') {
                    sum++;
                }
            } else {
                direction = left;
            }
        } else if (direction == left) {
            if (maps[x][y - 1] != '#') {
                y -= 1;
                if (maps[x][y] != 'x') {
                    sum++;
                }
            } else {
                direction = up;
            }
        }
        maps[x][y] = 'x';
        if (x == 0 || y==0 || x == row - 1 || y == col - 1) {
            break;
        }
    }

    return sum;
}

struct FootPrint {
    int x;
    int y;
    int direction;
};

bool operator==(const FootPrint& lhs, const FootPrint& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.direction == rhs.direction;
}

int GetObstaclePositionCount() {
    std::ifstream file("/Users/bytedance/workspace/CodeSnippet/advent_code_2024/06.txt"); // 打开文件
    if (!file) {
        return -1;
    }
    std::string line;
    std::vector<std::vector<char>> maps;
    int src_x = -1, src_y = -1;
    int i = 0;
    while(std::getline(file, line)) {
        std::vector<char> this_line;
        for(int j = 0; j < line.size(); j++) {
            if (line[j] == '^') {
                src_x = i;
                src_y = j;
                this_line.push_back('.');
            } else {
                this_line.push_back(line[j]);
            }
        }
        maps.push_back(this_line);
        i++;
    }
    int row = maps.size();
    int col = maps[0].size();

    std::vector<std::pair<int, int>> directions = {{-1, 0},{0,1},{1,0}, {0,-1}};

    int option_count = 0;
    for (int r = 0; r < row; r++) {
        for (int c = 0; c < col; c++) {
            int x = src_x;
            int y = src_y;
            int direction = 0;
            std::vector<FootPrint> visited;
            std::cout << "r: " << r << ", c: " << c << std::endl;
            while(true) {
                FootPrint cur = {x, y, direction};

                if (std::find(visited.begin(), visited.end(), cur) != visited.end()) {
                    option_count++;
                    break;
                }

                visited.push_back(cur);

                int new_x = x + directions[direction].first;
                int new_y = y + directions[direction].second;

                if (!(new_x >= 0 && new_x < row && new_y >= 0 && new_y < col)) {
                    break;
                }

                if (maps[new_x][new_y] == '#' || (new_x == r && new_y == c)) {
                    direction = (direction + 1) % 4;
                } else {
                    x = new_x;
                    y = new_y;
                }
            }
        }
    }
    return option_count;
}

#endif //INC_06_H
