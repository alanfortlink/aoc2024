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
      views::iota(size_t{0}, matrix.size() * matrix[0].size()) |
          views::transform([&](int p) {
            return ranges::count_if(
                ranges::begin(directions), ranges::end(directions),
                [&](auto dir) {
                  return is_match(matrix, "XMAS", 0, p / matrix[0].size(),
                                  p % matrix[0].size(), dir);
                });
          }),
      0, plus<>{});
}

int q2(const vector<string> &matrix) {
  return ranges::fold_left(
      views::iota(size_t{0}, matrix.size() * matrix[0].size()) |
          views::transform([&](int p) {
            int i = p / matrix[0].size();
            int j = p % matrix[0].size();
            return (is_match(matrix, "MAS", 0, i + 1, j - 1, {-1, 1}) ||
                    is_match(matrix, "MAS", 0, i - 1, j + 1, {1, -1})) &&
                   (is_match(matrix, "MAS", 0, i - 1, j - 1, {1, 1}) ||
                    is_match(matrix, "MAS", 0, i + 1, j + 1, {-1, -1}));
          }),
      0, plus<>{});
}

int main() {
  fstream fs("../input.txt");
  vector<string> matrix;
  string line;
  for (string line; fs >> line; matrix.push_back(line))
    ;
  cout << q1(matrix) << endl;
}
