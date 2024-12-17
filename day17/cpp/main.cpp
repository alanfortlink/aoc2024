#include <cmath>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

int64_t get_combo_operand(unordered_map<char, int> &registers,
                          int64_t operand) {
  if (operand >= 0 && operand <= 3) {
    return operand;
  }

  if (operand == 4) {
    return registers['A'];
  }

  if (operand == 5) {
    return registers['B'];
  }

  if (operand == 6) {
    return registers['C'];
  }

  throw runtime_error("Unexpected operand: " + to_string(operand));
}

void adv(unordered_map<char, int> &registers,
         const vector<int64_t> &instructions, int64_t &pc,
         vector<int64_t> &output) {
  int64_t numerator = registers['A'];
  int64_t combo_operand = get_combo_operand(registers, instructions[pc + 1]);
  int64_t denominator = pow(2, combo_operand);
  registers['A'] = numerator / denominator;
  pc += 2;
}

void bxl(unordered_map<char, int> &registers,
         const vector<int64_t> &instructions, int64_t &pc,
         vector<int64_t> &output) {
  int64_t lhs = registers['B'];
  int64_t rhs = instructions[pc + 1];
  registers['B'] = lhs ^ rhs;
  pc += 2;
}

void bst(unordered_map<char, int> &registers,
         const vector<int64_t> &instructions, int64_t &pc,
         vector<int64_t> &output) {
  registers['B'] = get_combo_operand(registers, instructions[pc + 1]) % 8;
  pc += 2;
}

void jnz(unordered_map<char, int> &registers,
         const vector<int64_t> &instructions, int64_t &pc,
         vector<int64_t> &output) {
  if (registers['A'] == 0) {
    pc += 2;
    return;
  }

  pc = instructions[pc + 1];
}

void bxc(unordered_map<char, int> &registers,
         const vector<int64_t> &instructions, int64_t &pc,
         vector<int64_t> &output) {
  registers['B'] = registers['B'] ^ registers['C'];
  pc += 2;
}

void out(unordered_map<char, int> &registers,
         const vector<int64_t> &instructions, int64_t &pc,
         vector<int64_t> &output) {
  output.push_back(get_combo_operand(registers, instructions[pc + 1]) % 8);
  pc += 2;
}

void bdv(unordered_map<char, int> &registers,
         const vector<int64_t> &instructions, int64_t &pc,
         vector<int64_t> &output) {
  int64_t numerator = registers['A'];
  int64_t combo_operand = get_combo_operand(registers, instructions[pc + 1]);
  int64_t denominator = pow(2, combo_operand);
  registers['B'] = numerator / denominator;
  pc += 2;
}

void cdv(unordered_map<char, int> &registers,
         const vector<int64_t> &instructions, int64_t &pc,
         vector<int64_t> &output) {
  int64_t numerator = registers['A'];
  int64_t combo_operand = get_combo_operand(registers, instructions[pc + 1]);
  int64_t denominator = pow(2, combo_operand);
  registers['C'] = numerator / denominator;
  pc += 2;
}

vector<int64_t> part1(unordered_map<char, int> &registers,
                      const vector<int64_t> &instructions) {
  vector<int64_t> output;

  void (*instructions_impl[])(unordered_map<char, int> &registers,
                              const vector<int64_t> &instructions, int64_t &pc,
                              vector<int64_t> &output) = {adv, bxl, bst, jnz,
                                                          bxc, out, bdv, cdv};

  int64_t pc = 0;
  while (pc < instructions.size()) {
    instructions_impl[instructions[pc]](registers, instructions, pc, output);
  }

  return output;
}

vector<int64_t>
part2_brute_force(const unordered_map<char, int> &original_registers,
                  const vector<int64_t> &instructions) {
  vector<int64_t> output;

  void (*instructions_impl[])(
      unordered_map<char, int> &, const vector<int64_t> &, int64_t &,
      vector<int64_t> &) = {adv, bxl, bst, jnz, bxc, out, bdv, cdv};

  for (int64_t p = 0;; p++) {
    cout << p << '\n';
    output.clear();

    unordered_map<char, int> registers(original_registers.begin(),
                                       original_registers.end());
    registers['A'] = p;

    int64_t pc = 0;
    while (pc < instructions.size()) {
      instructions_impl[instructions[pc]](registers, instructions, pc, output);

      if (!output.empty()) {
        int64_t last = output.size() - 1;
        if (output[last] != instructions[last]) {
          break;
        }
      }
    }

    if (output.size() == instructions.size()) {
      return output;
    }
  }

  return output;
}

int64_t part2_reverse_engineering(const vector<int64_t> &program, int64_t index,
                                  int64_t result) {
  if (index == program.size()) {
    return result;
  }

  for (int64_t cand = 0; cand < 8; cand++) {
    int64_t a = (result << 3) + cand;
    int64_t b = a % 8;
    b = b ^ 5;
    int64_t c = a / pow(2, b);
    // a = a / pow(2, 3);
    b = b ^ 6;
    b = b ^ c;
    if (b % 8 == program[program.size() - index - 1]) {
      int64_t rc = part2_reverse_engineering(program, index + 1, a);
      if (rc != -1) {
        return rc;
      }
    }
  }

  return -1;
}

int main(int argc, char **argv) {
  fstream fs(argc >= 2 ? argv[1] : "../input.txt");

  unordered_map<char, int> registers;

  string line_buffer;
  while (getline(fs, line_buffer)) {
    if (line_buffer.empty()) {
      break;
    }

    char reg;
    int value;
    sscanf(line_buffer.c_str(), "Register %c: %d", &reg, &value);
    registers[reg] = value;
  }

  vector<int64_t> instructions;

  for (string program_buffer; getline(fs, line_buffer);) {
    stringstream ss(line_buffer);
    ss >> program_buffer; // "Program:"
    while (getline(ss, program_buffer, ',')) {
      instructions.push_back(stoi(program_buffer));
    }
  }

  vector<int64_t> p1 = part1(registers, instructions);
  int64_t p2 = part2_reverse_engineering(instructions, 0, 0);
  auto joined = ranges::views::join_with(
      p1 | ranges::views::transform([](auto v) { return to_string(v); }), ",");

  stringstream ss;
  for (char c : joined) {
    ss << c;
  }

  cout << ss.str() << endl;
  cout << p2 << endl;
}
