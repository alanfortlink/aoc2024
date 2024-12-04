#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>

using namespace std;

bool is_match(const vector<string> &matrix, const string &word, int index,
              int i, int j, const pair<int, int> &dir) {
  if (index == word.size()) {
    return true;
  }

  if (i < 0 || j < 0 || i >= matrix.size() || j >= matrix[0].size()) {
    return false;
  }

  return matrix[i][j] == word[index] &&
         is_match(matrix, word, index + 1, i + dir.first, j + dir.second, dir);
}

int q1(const vector<string> &matrix) {
  const vector<pair<int, int>> directions = {
      {0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1},
  };

  return ranges::fold_left(
      ranges::views::iota(static_cast<size_t>(0), matrix.size()) |
          ranges::views::transform([&](int i) {
            return ranges::fold_left(
                ranges::views::iota(static_cast<size_t>(0), matrix[i].size()) |
                    ranges::views::transform([&](int j) {
                      return ranges::distance(
                          directions | ranges::views::filter([&](auto dir) {
                            return is_match(matrix, "XMAS", 0, i, j, dir);
                          }));
                    }),
                0, plus<>{});
          }),
      0, plus<>{});
}

int q2(const vector<string> &matrix) {
  int count = 0;
  for (size_t i = 0; i < matrix.size(); i++) {
    for (size_t j = 0; j < matrix.size(); j++) {
      if ((is_match(matrix, "MAS", 0, i + 1, j - 1, {-1, 1}) ||
           is_match(matrix, "MAS", 0, i - 1, j + 1, {1, -1})) &&
          (is_match(matrix, "MAS", 0, i - 1, j - 1, {1, 1}) ||
           is_match(matrix, "MAS", 0, i + 1, j + 1, {-1, -1}))) {
        count += 1;
      }
    }
  }

  return count;
}

int main() {
  fstream fs("../input.txt");
  vector<string> matrix;
  string line;
  for (string line; fs >> line; matrix.push_back(line))
    ;
  cout << q2(matrix) << endl;
}
