#include <cstdint>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <utility>

using namespace std;

int digits(uint64_t n) { return (n < 10) ? 1 : 1 + digits(n / 10); }

pair<uint64_t, uint64_t> split_number(uint64_t n, int m) {
  uint64_t rhs = 0;
  for (int i = 0; i < m; i++, n /= 10) {
    rhs += (n % 10) * pow(10, i);
  }
  return {n, rhs};
}

typedef pair<uint64_t, int> cpair;
auto hash_pair = [](const cpair &v) { return v.first ^ v.second; };

uint64_t calc(uint64_t n, int blinks, int limit) {
  static unordered_map<cpair, uint64_t, decltype(hash_pair)> C;

  const cpair key{n, blinks};

  if (C.contains(key)) {
    return C[key];
  }

  if (blinks == limit)
    return 1;
  if (n == 0)
    return C[key] = calc(1, blinks + 1, limit);

  int d = digits(n);
  if (d % 2 == 0) {
    const auto &[lhs, rhs] = split_number(n, d / 2);
    return C[key] = calc(lhs, blinks + 1, limit) + calc(rhs, blinks + 1, limit);
  }

  return C[key] = calc(n * 2024, blinks + 1, limit);
}

int main(int argc, char **argv) {
  fstream fs(argc == 2 ? argv[1] : "../example.txt");

  uint64_t total = 0;
  int blinks = 75;
  for (uint64_t num; fs >> num; total += calc(num, 0, blinks))
    ;

  cout << total << endl;
}
