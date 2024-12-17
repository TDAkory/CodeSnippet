//
// Created by ByteDance on 2024/12/3.
//

#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <ostream>
#include <vector>

namespace advent_of_code_2024_util {

template <typename T>
void PrintVec(const std::vector<T>& vec) {
  for (int i = 0; i < vec.size(); i++) {
    std::cout << vec[i] << " ";
  }
  std::cout << std::endl;
}

struct Point {
  int x{0};
  int y{0};

  Point(int _x, int _y) : x(_x), y(_y) {}

  bool operator==(const Point& rhs) const {
    return x == rhs.x && y == rhs.y;
  }

  bool operator<(const Point& rhs) const {
    if (x == rhs.x) {
      return y < rhs.y;
    }
    return x < rhs.x;
  }
};

using Matrix = std::vector<std::vector<Point>>;

using Points = std::vector<Point>;

static Points g_directions = {
  {-1,0},{0,1},{1,0},{0,-1},
};

}

#endif //UTIL_H
