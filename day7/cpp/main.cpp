#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;
using namespace std;

struct state {
  cpp_int current;
  size_t index;
};

bool q1(const vector<cpp_int> &nums, cpp_int target) {
  queue<state> q{};
  q.push(state{nums[0], 0});

  while (!q.empty()) {
    state s = std::move(q.front());
    q.pop();

    if (s.current == target && s.index == nums.size() - 1) {
      return true;
    }

    if (s.current > target || s.index == nums.size()) {
      continue;
    }

    q.push(state{s.current + nums[s.index + 1], s.index + 1});
    q.push(state{s.current * nums[s.index + 1], s.index + 1});
  }

  return false;
}

bool q2(const vector<cpp_int> &nums, cpp_int target) {
  queue<state> q{};
  q.push(state{nums[0], 0});

  while (!q.empty()) {
    state s = std::move(q.front());
    q.pop();

    if (s.current == target && s.index == nums.size() - 1) {
      return true;
    }

    if (s.current > target || s.index == nums.size() - 1) {
      continue;
    }

    const cpp_int next = nums[s.index + 1];

    q.push(state{s.current + next, s.index + 1});
    q.push(state{s.current * next, s.index + 1});
    q.push(state{cpp_int(to_string(s.current) + to_string(next)), s.index + 1});
  }

  return false;
}

int main(int argc, char *argv[]) {
  fstream fs(argv[1]);
  cpp_int ans = 0;
  for (string line; getline(fs, line);) {
    stringstream ss(line);
    string buf;

    ss >> buf;
    const cpp_int target = cpp_int(buf.substr(0, buf.size() - 1));
    vector<cpp_int> numbers;

    while (ss >> buf) {
      numbers.push_back(cpp_int(buf));
    }

    if (q2(numbers, target)) {
      ans += target;
    }
  }

  cout << ans << endl;
}