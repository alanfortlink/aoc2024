#include <algorithm>
#include <bits/ranges_algo.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ostream>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
using namespace std;

struct coord {
  int row;
  int col;
};
bool operator==(const coord &lhs, const coord &rhs) {
  return lhs.row == rhs.row && lhs.col == rhs.col;
}
coord operator+(const coord &lhs, const coord &rhs) {
  return {lhs.row + rhs.row, lhs.col + rhs.col};
}
ostream &operator<<(ostream &os, const coord &c) {
  os << "(" << c.row << ", " << c.col << ")";
  return os;
}
auto coord_hash = [](const coord &c) { return c.row ^ c.col; };
typedef unordered_set<coord, decltype(coord_hash)> coord_set;
typedef unordered_map<coord, bool, decltype(coord_hash)> coord_map;

// bool = "is_left"
typedef unordered_map<coord, bool, decltype(coord_hash)> box_map;

coord get_dir(char c) {
  switch (c) {
  case '>':
    return {0, 1};
  case '<':
    return {0, -1};
  case '^':
    return {-1, 0};
  case 'v':
    return {1, 0};
  default:
    throw runtime_error("unexpected value: " + to_string(c));
  }
}

bool can_clear_way(const coord &pos, const coord &dir, const coord_set &walls,
                   box_map &boxes, coord_map &seen) {
  if (walls.contains(pos)) {
    return false;
  }

  if (boxes.contains(pos)) {
    const bool is_vertical = dir == coord{-1, 0} || dir == coord{1, 0};
    const bool is_left = boxes[pos];

    const coord other_pos =
        is_left ? coord{pos.row, pos.col + 1} : coord{pos.row, pos.col - 1};

    if (is_vertical) {
      return can_clear_way(pos + dir, dir, walls, boxes, seen) &&
             can_clear_way(other_pos + dir, dir, walls, boxes, seen);
    }

    const bool going_right = dir == coord{0, 1};

    if (is_left && going_right) {
      return can_clear_way(other_pos + dir, dir, walls, boxes, seen);
    }

    if (is_left && !going_right) {
      return can_clear_way(pos + dir, dir, walls, boxes, seen);
    }

    if (!is_left && going_right) {
      return can_clear_way(pos + dir, dir, walls, boxes, seen);
    }

    if (!is_left && !going_right) {
      return can_clear_way(other_pos + dir, dir, walls, boxes, seen);
    }
  }

  return true;
}

void clear_way(const coord &pos, const coord &dir, const coord_set &walls,
               box_map &boxes, coord_set &seen) {
  if (walls.contains(pos)) {
    return;
  }

  if (boxes.contains(pos)) {
    const bool is_vertical = dir == coord{-1, 0} || dir == coord{1, 0};
    const bool is_left = boxes[pos];

    const coord other_pos =
        is_left ? coord{pos.row, pos.col + 1} : coord{pos.row, pos.col - 1};

    if (is_vertical) {
      clear_way(pos + dir, dir, walls, boxes, seen);
      clear_way(other_pos + dir, dir, walls, boxes, seen);

      bool bpos = boxes[pos];
      bool bopos = boxes[other_pos];

      boxes.erase(pos);
      boxes.erase(other_pos);

      boxes[pos + dir] = bpos;
      boxes[other_pos + dir] = bopos;
      return;
    }

    const bool going_right = dir == coord{0, 1};

    bool bpos = boxes[pos];
    bool bopos = boxes[other_pos];

    boxes.erase(pos);
    boxes.erase(other_pos);

    if (is_left && going_right) {
      clear_way(other_pos + dir, dir, walls, boxes, seen);
    }

    if (is_left && !going_right) {
      clear_way(pos + dir, dir, walls, boxes, seen);
    }

    if (!is_left && going_right) {
      clear_way(pos + dir, dir, walls, boxes, seen);
    }

    if (!is_left && !going_right) {
      clear_way(other_pos + dir, dir, walls, boxes, seen);
    }

    boxes[pos + dir] = bpos;
    boxes[other_pos + dir] = bopos;
  }
}

void print(char m, const coord &robot, const coord_set &walls,
           const box_map &boxes, const coord &grid_size) {
  cout << "Move: " << m << endl;
  for (int r = 0; r < grid_size.row; r++) {
    for (int c = 0; c < grid_size.col; c++) {
      coord p{r, c};

      char ch = '.';

      if (p == robot) {
        ch = '@';
      } else if (walls.contains(p)) {
        ch = '#';
      } else if (boxes.contains(p)) {
        if (boxes.find(p)->second) {
          ch = '[';
        } else {
          ch = ']';
        }
      }

      cout << ch;
    }
    cout << endl;
  }
  cout << endl;
}

void simulate(const string &moves, const coord &robot, const coord_set &walls,
              box_map &boxes, coord grid_size) {
  coord pos = robot;
  for (char m : moves) {
    coord dir = get_dir(m);
    coord new_pos = pos + dir;
    coord_map seen_map;
    if (can_clear_way(new_pos, dir, walls, boxes, seen_map)) {
      coord_set seen;
      clear_way(new_pos, dir, walls, boxes, seen);
      pos = new_pos;
    }
  }
}

int main(int argc, char **argv) {
  fstream fs(argc == 2 ? argv[1] : "../small_example.txt");

  coord robot;
  coord_set walls;
  box_map boxes;

  int row = 0;

  coord grid_size{0, 0};

  // read map
  for (string line; getline(fs, line); row++) {
    if (line.empty()) {
      break;
    }

    for (int col = 0; col < line.size(); col++) {
      coord p{row, col * 2};
      coord pn{row, col * 2 + 1};
      const char c = line[col];

      if (pn.col + 1 > grid_size.col) {
        grid_size.col = pn.col + 1;
      }

      if (c == '.') {
        continue;
      }

      if (c == '#') {
        walls.insert(p);
        walls.insert(pn);
      } else if (c == 'O') {
        boxes[p] = true;
        boxes[pn] = false;
      } else if (c == '@') {
        robot = p;
      }
    }
  }

  grid_size.row = row;

  // reading moves
  stringstream moves_ss;
  for (string line; getline(fs, line); moves_ss << line)
    ;

  const string &moves = moves_ss.str();
  simulate(moves, robot, walls, boxes, grid_size);

  const int total = ranges::fold_left(
      boxes | ranges::views::transform([](const pair<coord, bool> &box) {
        if (!box.second)
          return 0;
        return box.first.row * 100 + box.first.col;
      }),
      0, plus<int>{});

  cout << total << endl;
}
