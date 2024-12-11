#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

using namespace std;
struct v2;

struct v2 {
  int i, j;
  inline v2 operator+(const v2 &rhs) { return v2{i + rhs.i, j + rhs.j}; }
};

inline bool operator==(const v2 &lhs, const v2 &rhs) {
  return lhs.i == rhs.i && lhs.j == rhs.j;
}

struct v2_hash {
  inline std::size_t operator()(const v2 &v) const {
    return v.i * 130 + v.j;
  }
};

struct pair_hash {
  inline std::size_t operator()(const std::pair<v2, v2> &v) const {
    return v.first.i * 600 + v.first.j * 300 + v.second.i * 100 + v.second.j;
  }
};

v2 rotated(const v2 &v) {
  static const vector<const v2> C{{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
  const int index = distance(C.begin(), find(C.begin(), C.end(), v));
  return C[(index + 1) % C.size()];
}

bool is_in_bounds(const vector<vector<char>> &m, v2 p) {
  return p.i >= 0 && p.i < m.size() && p.j >= 0 && p.j < m[p.i].size();
}

unordered_set<v2, v2_hash> get_path(const vector<vector<char>> &matrix, v2 p, v2 d) {
  unordered_set<v2, v2_hash> seen;

  while (is_in_bounds(matrix, p)) {
    seen.insert(p);
    v2 next = p + d;
    if (is_in_bounds(matrix, next)) {
      if (matrix[next.i][next.j] == '#') {
        d = rotated(d);
      } else {
        p = next;
      }
    } else {
      return seen;
    }
  }

  return seen;
}

int count_cycle(const vector<vector<char>> &matrix, v2 p, v2 d) {
  unordered_set<pair<v2, v2>, pair_hash> seen;
  while (is_in_bounds(matrix, p)) {
    if (seen.contains({p, d})) {
      return 1;
    }

    seen.insert({p, d});

    v2 next = p + d;
    if (is_in_bounds(matrix, next)) {
      if (matrix[next.i][next.j] == '#') {
        d = rotated(d);
      } else {
        p = next;
      }
    } else {
      return 0;
    }
  }

  return 0;
}

int main(int argc, char *argv[]) {
  fstream fs(argv[1]);
  vector<vector<char>> matrix;
  string line;
  v2 p{0, 0};
  for (int i = 0; fs >> line;
       i++, matrix.push_back(vector<char>(line.begin(), line.end()))) {
    const int j = line.find('^');
    if (j != string::npos) {
      p = v2{i, j};
    }
  }

  auto cycle = get_path(matrix, p, v2{-1, 0});

  int c{0};
  for (auto [i, j] : cycle) {
    if (matrix[i][j] == '.') {
      matrix[i][j] = '#';
      c += count_cycle(matrix, p, v2{-1, 0});
      matrix[i][j] = '.';
    }
  }

  cout << c << endl;
}
