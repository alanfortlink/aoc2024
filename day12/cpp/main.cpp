#include <fstream>
#include <iostream>
#include <ostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

typedef pair<int, int> coord;
ostream &operator<<(ostream &out, const coord &e) {
  out << e.first << "," << e.second;
  return out;
}

typedef pair<coord, coord> edge;
ostream &operator<<(ostream &out, const edge &e) {
  out << e.first << "->" << e.second;
  return out;
}

auto coord_hash = [](const coord &v) { return v.first ^ v.second; };
auto edge_hash = [](const edge &e) {
  return coord_hash(e.first) ^ coord_hash(e.second);
};

typedef unordered_set<coord, decltype(coord_hash)> coord_set;
typedef unordered_set<edge, decltype(edge_hash)> edge_set;
typedef unordered_map<edge, unordered_set<coord, decltype(coord_hash)>,
                      decltype(edge_hash)>
    edge_map;

coord directions[] = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};

bool in_bounds(const vector<string> &grid, int i, int j) {
  return i >= 0 && i < grid.size() && j >= 0 && j < grid[i].size();
}

int calc_perimiter(const vector<string> &grid, const coord_set &region) {
  int open_sides = 0;
  for (const auto &[i, j] : region) {
    for (const auto &[di, dj] : directions) {
      const int ni = i + di;
      const int nj = j + dj;
      if (!in_bounds(grid, ni, nj) || grid[ni][nj] != grid[i][j]) {
        open_sides += 1;
      }
    }
  }

  return open_sides;
}

void remove_neighbors(edge_set &edges, edge_map &seen, edge e, coord dir) {
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

int calc_sides(const vector<string> &grid, const coord_set region) {
  edge_set edges;
  for (const auto &[i, j] : region) {
    for (const auto &[di, dj] : directions) {
      int ni = i + di;
      int nj = j + dj;

      if (region.contains({ni, nj})) {
        continue;
      }

      edge new_edge{{i, j}, {ni, nj}};
      edges.insert(new_edge);
    }
  }

  edge_map seen;

  int count = 0;
  for (const auto &e : edges) {
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

void get_region(const vector<string> &grid, char crop, int start_i, int start_j,
                coord_set &region) {
  stack<coord> s{};
  s.push({start_i, start_j});

  while (!s.empty()) {
    const auto [i, j] = s.top();
    s.pop();

    if (!in_bounds(grid, i, j)) {
      continue;
    }

    if (grid[i][j] != crop) {
      continue;
    }

    if (region.contains({i, j})) {
      continue;
    }

    region.insert({i, j});

    for (const auto &[di, dj] : directions) {
      s.push({i + di, j + dj});
    }
  }
}

int q1(vector<string> &grid) {
  int fencing_cost = 0;

  coord_set visited;
  for (int i = 0; i < grid.size(); i++) {
    for (int j = 0; j < grid[i].size(); j++) {
      if (visited.contains({i, j})) {
        continue;
      }

      char crop = grid[i][j];
      coord_set region;
      get_region(grid, crop, i, j, region);

      const int area = region.size();
      const int perimeter = calc_perimiter(grid, region);

      fencing_cost += area * perimeter;

      visited.insert(region.begin(), region.end());
    }
  }

  return fencing_cost;
}

int q2(vector<string> &grid) {
  int fencing_cost = 0;

  coord_set visited;
  for (int i = 0; i < grid.size(); i++) {
    for (int j = 0; j < grid[i].size(); j++) {
      if (visited.contains({i, j})) {
        continue;
      }

      char crop = grid[i][j];
      coord_set region;
      get_region(grid, crop, i, j, region);

      const int area = region.size();
      const int sides = calc_sides(grid, region);

      fencing_cost += area * sides;

      visited.insert(region.begin(), region.end());
    }
  }

  return fencing_cost;
}

int main(int argc, char **argv) {
  fstream fs(argc == 2 ? argv[1] : "../first_example.txt");
  vector<string> grid;
  for (string line; fs >> line; grid.push_back(line))
    ;

  cout << q1(grid) << endl;
  cout << q2(grid) << endl;
}
