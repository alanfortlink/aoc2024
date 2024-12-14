#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <ratio>
#include <thread>
#include <unistd.h>
#include <unordered_map>
#include <vector>
using namespace std;

struct v2 {
  int x;
  int y;
};

struct robot {
  v2 position;
  v2 speed;

  robot(const string &encoded_robot) {
    sscanf(encoded_robot.c_str(), "p=%d,%d v=%d,%d", &position.x, &position.y,
           &speed.x, &speed.y);
  }
};

v2 get_xy(const robot &r, int seconds, v2 grid_size) {
  int nx = r.position.x + r.speed.x * seconds;
  int ny = r.position.y + r.speed.y * seconds;

  int ux = nx % grid_size.x;
  int uy = ny % grid_size.y;

  if (ux < 0) {
    ux = grid_size.x + ux;
  }

  if (uy < 0) {
    uy = grid_size.y + uy;
  }

  return {ux, uy};
}

int get_quadrant(const robot &r, int seconds, v2 grid_size) {
  auto [ux, uy] = get_xy(r, seconds, grid_size);

  const int mid_x = grid_size.x / 2;
  const int mid_y = grid_size.y / 2;

  if (ux == mid_x || uy == mid_y) {
    return -1;
  }

  if (ux < mid_x && uy < mid_y) {
    return 0;
  }

  if (ux < mid_x && uy > mid_y) {
    return 1;
  }

  if (ux > mid_x && uy > mid_y) {
    return 2;
  }

  return 3;
}

int main_part1(int argc, char **argv) {
  fstream fs(argc == 2 ? argv[1] : "../example.txt");

  vector<int> quadrants(4, 0);

  for (string line; getline(fs, line);) {
    robot r(line);
    const int quad = get_quadrant(r, 100, {101, 103});
    if (quad != -1) {
      quadrants[quad]++;
    }
  }

  cout << (quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3]) << endl;
  return 0;
}

// SIMULATION

void render(const vector<vector<char>> &canvas) {
  for (int i = 0; i < canvas.size(); i++) {
    for (int j = 0; j < canvas[i].size(); j++) {
      cout << canvas[i][j];
    }
    cout << endl;
  }
  cout << endl;
}

// SIMULATION
int main_part2(int argc, char **argv) {
  fstream fs(argc == 2 ? argv[1] : "../example.txt");

  vector<robot> robots;
  for (string line; getline(fs, line);) {
    robots.push_back(line);
  }

  v2 grid_size{101, 103};
  vector<vector<char>> canvas(grid_size.y, vector<char>(grid_size.x, ' '));

  for (int seconds = 0; true; seconds++) {
    for (auto &row : canvas) {
      for (auto &col : row) {
        col = ' ';
      }
    }

    unordered_map<int, int> xs;
    unordered_map<int, int> ys;

    int max_xs = 0;
    int max_ys = 0;

    for (auto &r : robots) {
      auto [x, y] = get_xy(r, seconds, grid_size);
      canvas[y][x] = '#';
      xs[x]++;
      ys[y]++;

      max_xs = max(xs[x], max_xs);
      max_ys = max(ys[y], max_ys);
    }

    system("clear");
    render(canvas);
    cout << "After " << seconds << " seconds" << endl;

    int threshold = 30;
    if (max_xs > threshold || max_ys > threshold) {
      getchar();
    }
  }
}

int main(int argc, char **argv) { return main_part2(argc, argv); }
