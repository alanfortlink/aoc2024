#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

typedef pair<int, int> coord;

coord get_prize_coord(const string &encoded_prize) {
  coord c;
  sscanf(encoded_prize.c_str(), "Prize: X=%d, Y=%d", &c.first, &c.second);
  return c;
}

struct instruction {
  char button;
  coord delta;

  instruction(const string &encoded_instruction) {
    sscanf(encoded_instruction.c_str(), "Button %c: X+%d, Y+%d", &button,
           &delta.first, &delta.second);
  }

  int tokens() const { return button == 'A' ? 3 : 1; }
};

struct machine {
  static int mid;
  const int id;
  const instruction a;
  const instruction b;
  const coord prize_location;

  machine(const vector<string> &encoded_instructions)
      : id(mid++), a(instruction(encoded_instructions[0])),
        b(instruction(encoded_instructions[1])),
        prize_location(get_prize_coord(encoded_instructions[2])) {}
};

int machine::mid = 0;

int q2(const machine &m) {
  const int INF = 1000;
  int cheapest = INF;

  for (int x = 0; x <= 100; x++) {
    for (int y = 0; y <= 100; y++) {
      const int tx = x * m.a.delta.first + y * m.b.delta.first;
      const int ty = x * m.a.delta.second + y * m.b.delta.second;

      if (m.prize_location == coord{tx, ty}) {
        const int cost = x * m.a.tokens() + y * m.b.tokens();
        cheapest = min(cheapest, cost);
      }
    }
  }

  return cheapest == INF ? 0 : cheapest;
}

int main(int argc, char **argv) {
  fstream fs(argc == 2 ? argv[1] : "../example.txt");
  vector<string> encoded_instructions;
  int total = 0;
  for (string line; getline(fs, line);) {
    if (!line.empty()) {
      encoded_instructions.push_back(line);
      continue;
    }

    total += q2(encoded_instructions);
    encoded_instructions.clear();
  }

  total += q2(encoded_instructions);

  cout << total << endl;
}
