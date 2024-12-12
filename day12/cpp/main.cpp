#include <fstream>
#include <iostream>
#include <ostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

typedef pair<pair<int, int>, pair<int, int>> edge;

ostream &operator<<(ostream &out, const pair<int, int> &e) {
  out << e.first << "," << e.second;
  return out;
}

ostream &operator<<(ostream &out, const edge &e) {
  out << e.first << "->" << e.second;
  return out;
}

auto pair_hash = [](const pair<int, int> &v) { return v.first ^ v.second; };
auto edge_hash = [](const edge &e) {
  return pair_hash(e.first) ^ pair_hash(e.second);
};

auto edge_dir_hash = [](const pair<edge, pair<int, int>> &e) {
  return edge_hash(e.first) ^ pair_hash(e.second);
};

typedef unordered_set<pair<int, int>, decltype(pair_hash)> custom_set;
typedef unordered_set<edge, decltype(edge_hash)> custom_edge_s;
typedef unordered_map<edge, unordered_set<pair<int, int>, decltype(pair_hash)>,
                      decltype(edge_hash)>
    custom_edge_map;

pair<int, int> N = {-1, 0};
pair<int, int> S = {1, 0};
pair<int, int> E = {0, 1};
pair<int, int> W = {0, -1};
pair<int, int> directions[] = {N, S, E, W};

bool in_bounds(const vector<string> &grid, int i, int j) {
  return i >= 0 && i < grid.size() && j >= 0 && j < grid[i].size();
}

int calc_perimiter(const vector<string> &grid, const custom_set &visited) {
  int open_sides = 0;
  for (const auto [i, j] : visited) {
    for (const auto [di, dj] : directions) {
      const int ni = i + di;
      const int nj = j + dj;
      if (!in_bounds(grid, ni, nj) || grid[ni][nj] != grid[i][j]) {
        open_sides += 1;
      }
    }
  }

  return open_sides;
}

void remove_neighbors(custom_edge_s &edges, custom_edge_map &seen, edge e,
                      pair<int, int> dir) {
  if (seen[e].contains(dir)) {
    return;
  }

  if (!edges.contains(e)) {
    return;
  }

  seen[e].insert(dir);

  edge ne = {{e.first.first + dir.first, e.first.second + dir.second},
             {e.second.first + dir.first, e.second.second + dir.second}};

  remove_neighbors(edges, seen, ne, dir);
}

int calc_sides(const vector<string> &grid, const custom_set region) {
  custom_edge_s edges;
  for (auto [i, j] : region) {
    for (auto [di, dj] : directions) {
      int ni = i + di;
      int nj = j + dj;

      if (region.contains({ni, nj})) {
        continue;
      }

      edge new_edge{{i, j}, {ni, nj}};
      edges.insert(new_edge);
    }
  }

  custom_edge_map seen;

  int count = 0;
  for (auto e : edges) {
    if (seen.contains(e)) {
      continue;
    }

    count++;

    if (e.first.first == e.second.first) {
      remove_neighbors(edges, seen, e, {1, 0});
      remove_neighbors(edges, seen, e, {-1, 0});
    } else {
      remove_neighbors(edges, seen, e, {0, 1});
      remove_neighbors(edges, seen, e, {0, -1});
    }
  }

  return count;
}

void walk(const vector<string> &grid, char crop, int start_i, int start_j,
          custom_set &visited) {
  stack<pair<int, int>> s{};
  s.push({start_i, start_j});

  while (!s.empty()) {
    auto [i, j] = s.top();
    s.pop();

    if (!in_bounds(grid, i, j)) {
      continue;
    }

    if (grid[i][j] != crop) {
      continue;
    }

    if (visited.contains({i, j})) {
      continue;
    }

    visited.insert({i, j});

    for (const auto [di, dj] : directions) {
      s.push({i + di, j + dj});
    }
  }
}

int q1(vector<string> &grid) {
  int fencing_cost = 0;

  custom_set all_visited;
  for (int i = 0; i < grid.size(); i++) {
    for (int j = 0; j < grid[i].size(); j++) {
      if (all_visited.contains({i, j})) {
        continue;
      }

      char crop = grid[i][j];
      custom_set visited;
      walk(grid, crop, i, j, visited);

      const int area = visited.size();
      const int perimeter = calc_perimiter(grid, visited);

      fencing_cost += area * perimeter;

      for (auto v : visited) {
        all_visited.insert(v);
      }
    }
  }

  return fencing_cost;
}

int q2(vector<string> &grid) {
  int fencing_cost = 0;

  custom_set all_visited;
  for (int i = 0; i < grid.size(); i++) {
    for (int j = 0; j < grid[i].size(); j++) {
      if (all_visited.contains({i, j})) {
        continue;
      }

      char crop = grid[i][j];
      custom_set visited;
      walk(grid, crop, i, j, visited);

      const int area = visited.size();
      const int sides = calc_sides(grid, visited);

      fencing_cost += area * sides;

      for (auto v : visited) {
        all_visited.insert(v);
      }
    }
  }

  return fencing_cost;
}

int main(int argc, char **argv) {
  fstream fs(argc == 2 ? argv[1] : "../first_example.txt");
  vector<string> grid;
  for (string line; fs >> line; grid.push_back(line))
    ;

  int fencing_cost = q1(grid);
  cout << fencing_cost << endl;
}
