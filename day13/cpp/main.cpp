#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

typedef pair<int64_t, int64_t> coord;
coord get_prize_coord(const string &encoded_prize, int64_t inc) {
  coord c;
  sscanf(encoded_prize.c_str(), "Prize: X=%ld, Y=%ld", &c.first, &c.second);
  c.first += inc;
  c.second += inc;
  return c;
}

struct instruction {
  char button;
  coord delta;

  int64_t x() const { return delta.first; }
  int64_t y() const { return delta.second; }

  instruction(const string &encoded_instruction) {
    sscanf(encoded_instruction.c_str(), "Button %c: X+%ld, Y+%ld", &button,
           &delta.first, &delta.second);
  }

  int tokens() const { return button == 'A' ? 3 : 1; }
};

struct machine {
  const instruction a;
  const instruction b;
  const coord p;

  int64_t x() const { return p.first; };
  int64_t y() const { return p.second; };

  machine(const vector<string> &encoded_instructions, int64_t inc)
      : a(instruction(encoded_instructions[0])),
        b(instruction(encoded_instructions[1])),
        p(get_prize_coord(encoded_instructions[2], inc)) {}
};

int64_t count(const machine &m) {
  int64_t x_den = (m.a.x() * m.b.y() - m.b.x() * m.a.y());
  int64_t x_num = (m.x() * m.b.y() - m.b.x() * m.y());

  int64_t x = x_num / x_den;
  int64_t xr = x_num % x_den;

  if (xr != 0) {
    return 0;
  }

  int64_t y_den = m.b.y();
  int64_t y_num = (m.y() - m.a.y() * x);

  int64_t y = y_num / y_den;
  int64_t yr = y_num % y_den;

  if (yr != 0) {
    return 0;
  }

  return x * m.a.tokens() + y * m.b.tokens();
}

int main(int argc, char **argv) {
  fstream fs(argc >= 2 ? argv[1] : "../example.txt");
  vector<string> encoded_instructions;
  int64_t total = 0;

  const bool is_q2 = true;
  int64_t inc = is_q2 ? 10'000'000'000'000 : 0;

  for (string line; getline(fs, line);) {
    if (!line.empty()) {
      encoded_instructions.push_back(line);
      continue;
    }

    total += count(machine(encoded_instructions, inc));
    encoded_instructions.clear();
  }

  total += count(machine(encoded_instructions, inc));

  cout << total << endl;
}
