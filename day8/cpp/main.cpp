#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  fstream fs(argv[1]);
  vector<string> grid;
  for (string line; fs >> line; grid.push_back(line))
    ;
  unordered_map<char, vector<pair<int, int>>> antennas;
  for (size_t i{0}; i < grid.size(); i++) {
    for (size_t j{0}; j < grid[i].size(); j++) {
      if (grid[i][j] != '.') {
        antennas[grid[i][j]].push_back({i, j});
      }
    }
  }

  constexpr auto hash_pair = [](const pair<int, int> &v) -> int {
    return v.first * 130 + v.second;
  };

  unordered_set<pair<int, int>, decltype(hash_pair)> antinodes;

  auto in_bounds = [&](int i, int j) -> bool {
    return i >= 0 && i < grid.size() && j >= 0 && j < grid[i].size();
  };

  for (auto [f, p] : antennas) {
    for (int i = 0; i < p.size(); i++) {
      for (int j = 0; j < p.size(); j++) {
        if (i == j) {
          continue;
        }

        pair<int, int> itoj = {p[j].first - p[i].first,
                               p[j].second - p[i].second};
        int pi = p[i].first;
        int pj = p[i].second;

        while (in_bounds(pi, pj)) {
          antinodes.insert({pi, pj});
          pi = pi + itoj.first;
          pj = pj + itoj.second;
        }
      }
    }
  }

  cout << antinodes.size() << endl;
}
