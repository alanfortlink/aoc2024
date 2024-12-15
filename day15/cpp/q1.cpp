
#include <algorithm>
#include <bits/ranges_algo.h>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
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
auto coord_hash = [](const coord &c) { return c.row ^ c.col; };
typedef unordered_set<coord, decltype(coord_hash)> coord_set;

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

bool try_and_clear(const coord &pos, const coord &dir, const coord_set &walls,
                   coord_set &boxes) {
  if (walls.contains(pos)) {
    return false;
  }

  if (boxes.contains(pos)) {
    coord new_pos = pos + dir;
    if (try_and_clear(new_pos, dir, walls, boxes)) {
      boxes.erase(pos);
      boxes.insert(new_pos);
      return true;
    }

    return false;
  }

  return true;
}

void simulate(const string &moves, const coord &robot, const coord_set &walls,
              coord_set &boxes) {
  coord pos = robot;
  for (char m : moves) {
    coord dir = get_dir(m);
    coord new_pos = pos + dir;
    if (try_and_clear(new_pos, dir, walls, boxes)) {
      pos = new_pos;
    }
  }
}

int main(int argc, char **argv) {
  fstream fs(argc == 2 ? argv[1] : "../example.txt");

  coord robot;
  coord_set walls;
  coord_set boxes;

  int row = 0;

  // read map
  for (string line; getline(fs, line); row++) {
    if (line.empty()) {
      break;
    }

    for (int col = 0; col < line.size(); col++) {
      coord p{row, col};
      const char c = line[col];

      if (c == '.') {
        continue;
      }

      if (c == '#') {
        walls.insert(p);
      } else if (c == 'O') {
        boxes.insert(p);
      } else if (c == '@') {
        robot = p;
      }
    }
  }

  // reading moves
  stringstream moves_ss;
  for (string line; getline(fs, line); moves_ss << line)
    ;

  const string &moves = moves_ss.str();
  simulate(moves, robot, walls, boxes);

  const int total =
      ranges::fold_left(boxes | ranges::views::transform([](const coord &box) {
                          return box.row * 100 + box.col;
                        }),
                        0, plus<int>{});

  cout << total << endl;
}
