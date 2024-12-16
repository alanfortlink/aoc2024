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
struct state {
  coord p;
  int64_t acc;
  int dir;
  vector<coord> path;
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
int pixelsize = 5;

vector<uint8_t> print_board(const board &b, vector<coord> &min_path,
                            vector<state> &slr, const coord_map &path,
                            bool is_last) {
  int height = b.size();
  int width = b[0].size();
  vector<uint8_t> colors(width * height * 4 * pixelsize * pixelsize, 0);

  sort(slr.begin(), slr.end(),
       [](const state &lhs, const state &rhs) { return lhs.acc < rhs.acc; });

  coord_set cs(min_path.begin(), min_path.end());

  vector<coord_set> seen_last_round;
  int limit = min(size_t{300}, slr.size());
  for (int i = 0; i < limit; i++) {
    coord_set s(slr[i].path.begin(), slr[i].path.end());
    seen_last_round.push_back(s);
  }

  for (int mi = 0; mi < b.size() * pixelsize; mi++) {
    int i = mi / (pixelsize);
    for (int mj = 0; mj < b[i].size() * pixelsize; mj++) {
      int j = mj / (pixelsize);
      int mindex = (mi * width * pixelsize + mj) * 4;
      int index = (i * width + j) * 4;
      char c = b[i][j];

      auto set_color = [&](uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
        colors[mindex] = r;
        colors[mindex + 1] = g;
        colors[mindex + 2] = b;
        colors[mindex + 3] = a;
      };

      if (c == '#') {
        set_color(0, 0, 0);
        continue;
      }

      if (cs.contains({i, j})) {
        set_color(30, 255, 30);
        continue;
      }

      if (c == '.') {
        if (is_last) {
          set_color(0, 40, 0);
          continue;
        }

        if (seen_last_round.size() > 0) {
          if (seen_last_round[0].contains({i, j})) {
            set_color(0, 255, 0);
            continue;
          }
        }

        int n = limit;
        bool should_continue = false;
        for (int ii = 1; ii < n; ii++) {
          if (seen_last_round[ii].contains({i, j})) {
            set_color(0, 50 + 140 * ((double)(n - ii) / n), 0);
            should_continue = true;
            break;
          }
        }

        if (should_continue) {
          continue;
        }

        auto it = path.find({i, j});
        if (it != path.end()) {
          if (it->second.size() == 1) {
            set_color(255, 0, 0);
          }

          if (it->second.size() == 2) {
            set_color(255, 0, 0);
          }

          if (it->second.size() == 3) {
            set_color(255, 0, 0);
          }

          if (it->second.size() == 4) {
            set_color(0, 50, 0);
          }

          // int fac = ((double)it->second.size()) / 4.0;
          continue;
        }

        set_color(255, 255, 255);
        continue;
      }

      if (c == 'S') {
        set_color(0, 255, 0);
        continue;
      }

      if (c == 'E') {
        set_color(255, 0, 0);
        continue;
      }

      set_color(0, 0, 255);
    }
  }

  return colors;
}

pair<state, vector<coord>> part1(const coord &player, board &board,
                                 bool animate) {
  state min_state = state{player, numeric_limits<int>{}.max(), 0, {}};

  queue<state> q;
  q.push({player, 0, 0, {}});

  coord_map m;
  vector<coord> in_best_path;

  vector<uint8_t> colors;
  vector<state> seen_last_round;
  vector<vector<uint8_t>> frames;

  colors = print_board(board, in_best_path, seen_last_round, m, false);

  GifWriteFrame(&writer, colors.data(), board[0].size() * pixelsize,
                board.size() * pixelsize, 100);
  int count = q.size();

  while (!q.empty()) {
    auto s = q.front();
    q.pop();

    seen_last_round.push_back(s);

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

    count--;
    if (count == 0) {
      if (animate) {
        colors = print_board(board, in_best_path, seen_last_round, m, false);
        frames.push_back(colors);
      }
      count = q.size() + 1;
      seen_last_round.clear();
    }
  }

  if (animate) {
    cout << "Writing file" << endl;

    for (auto frame : frames) {
      GifWriteFrame(&writer, frame.data(), board[0].size() * pixelsize,
                    board.size() * pixelsize, 5);
    }
  }

  colors = print_board(board, in_best_path, seen_last_round, m, true);
  GifWriteFrame(&writer, colors.data(), board[0].size() * pixelsize,
                board.size() * pixelsize, 200);

  return {min_state, in_best_path};
}

int main(int argc, char **argv) {
  fstream fs(argc >= 2 ? argv[1] : "../example2.txt");

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

  GifBegin(&writer, "out.gif", b[0].size() * pixelsize, b.size() * pixelsize,
           200);

  auto [s, in_best_path] = part1(start, b, argc == 3);

  GifEnd(&writer);

  cout << s.acc << endl;
  coord_set part2(in_best_path.begin(), in_best_path.end());
  cout << part2.size() << endl;
}
