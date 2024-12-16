#include "gif.h"
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <ostream>
#include <queue>
#include <random>
#include <unordered_map>
#include <unordered_set>

using namespace std;

typedef vector<vector<char>> board;

struct coord {
  int r;
  int c;
};
ostream &operator<<(ostream &out, const coord &c) {
  out << "(" << c.r << ", " << c.c << ")";
  return out;
}
bool operator==(const coord &lhs, const coord &rhs) {
  return lhs.r == rhs.r && lhs.c == rhs.c;
}
coord operator+(const coord &lhs, const coord &rhs) {
  return {lhs.r + rhs.r, lhs.c + rhs.c};
}

auto coord_hash = [](const coord &c) { return c.r ^ c.c; };
typedef unordered_map<coord, unordered_map<int, int>, decltype(coord_hash)>
    coord_map;
typedef unordered_set<coord, decltype(coord_hash)> coord_set;

int64_t min(const vector<int64_t> &values) {
  int64_t m = values[0];
  for (int i = 1; i < values.size(); i++) {
    m = min(m, values[i]);
  }

  return m;
}

namespace {
coord E{0, 1};
coord S{1, 0};
coord W{0, -1};
coord N{-1, 0};
const coord D[4] = {E, S, W, N};
} // namespace

int rotated(int direction, int rotations) {
  return (direction + rotations) % 4;
}

GifWriter writer;

vector<uint8_t> print_board(const board &b, vector<coord> &in_best_path,
                            const coord_map &path) {
  int height = b.size();
  int width = b[0].size();
  vector<uint8_t> colors(width * height * 4, 0);

  auto set_color = [&](int i, uint8_t r, uint8_t g, uint8_t b) {
    colors[i] = r;
    colors[i + 1] = g;
    colors[i + 2] = b;
    colors[i + 3] = 255;
  };

  coord_set cs(in_best_path.begin(), in_best_path.end());

  for (int i = 0; i < b.size(); i++) {
    for (int j = 0; j < b[i].size(); j++) {
      int index = (i * width + j) * 4;
      char c = b[i][j];

      if (c == '#') {
        set_color(index, 0, 0, 0);
        continue;
      }

      if (c == '.') {
        auto it = path.find({i, j});
        if (it != path.end()) {
          int fac = ((double)it->second.size()) / 4.0;
          set_color(index, 100 * fac, 255 * fac, 100 * fac);
          continue;
        }

        set_color(index, 255, 255, 255);
        continue;
      }

      if (c == 'S') {
        set_color(index, 0, 255, 0);
        continue;
      }

      if (c == 'E') {
        set_color(index, 255, 0, 0);
        continue;
      }

      set_color(index, 0, 0, 255);
    }
  }

  GifWriteFrame(&writer, colors.data(), width, height, 0);
  return colors;
}

struct state {
  coord p;
  int64_t acc;
  int dir;
  vector<coord> path;
};
pair<state, vector<coord>> part1(const coord &player, board &board) {
  state min_state = state{player, numeric_limits<int>{}.max(), 0, {}};

  queue<state> q;
  q.push({player, 0, 0, {}});

  coord_map m;
  vector<coord> in_best_path;

  vector<uint8_t> colors;
  colors = print_board(board, in_best_path, m);

  GifWriteFrame(&writer, colors.data(), board[0].size(), board.size(), 100);
  int count = q.size();

  while (!q.empty()) {
    auto s = q.front();
    q.pop();

    if (s.acc > min_state.acc) {
      continue;
    }

    if (m[s.p].contains(s.dir)) {
      if (m[s.p][s.dir] < s.acc) {
        continue;
      }
    }

    m[s.p][s.dir] = s.acc;
    s.path.push_back(s.p);

    char c = board[s.p.r][s.p.c];

    if (c == '#') {
      continue;
    }

    if (c == 'E') {
      if (s.acc < min_state.acc) {
        min_state = s;
        in_best_path = s.path;
      } else if (s.acc == min_state.acc) {
        copy(s.path.begin(), s.path.end(), back_inserter(in_best_path));
      }

      continue;
    }

    count--;
    if (count == 0) {
      colors = print_board(board, in_best_path, m);
      count = q.size() + 1;
    }

    vector<state> states;
    states.push_back({s.p + D[s.dir], s.acc + 1, s.dir, s.path});
    states.push_back({s.p, s.acc + 1000, rotated(s.dir, 1), s.path});
    states.push_back({s.p, s.acc + 1000, rotated(s.dir, 3), s.path});

    random_device rd;
    std::mt19937 g(rd());

    shuffle(states.begin(), states.end(), g);

    for (auto s : states) {
      q.push(s);
    }
  }

  GifWriteFrame(&writer, colors.data(), board[0].size(), board.size(), 200);

  return {min_state, in_best_path};
}

int main(int argc, char **argv) {
  fstream fs(argc == 2 ? argv[1] : "../example2.txt");

  board b;
  coord start;

  int row = 0;
  for (string line; getline(fs, line);
       b.push_back(vector<char>(line.begin(), line.end())), row++) {
    int start_pos = line.find('S');
    if (start_pos != string::npos) {
      start = {row, start_pos};
    }
  }

  GifBegin(&writer, "out.gif", b[0].size(), b.size(), 200);

  auto [s, in_best_path] = part1(start, b);

  GifEnd(&writer);

  cout << s.acc << endl;
  coord_set part2(in_best_path.begin(), in_best_path.end());
  cout << part2.size() << endl;
}
