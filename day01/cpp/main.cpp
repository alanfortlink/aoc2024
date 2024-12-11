#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>

using namespace std;

void q1(fstream &fileStream);
void q2(fstream &fileStream);

int main(int argc, char **argv) {
  if (argc != 2) {
    cout << "Usage: ./" << argv[0] << " <file_location>" << endl;
    return 1;
  }

  cout << argv[1] << endl;
  auto fileStream = fstream(argv[1]);

  if (!fileStream) {
    cout << "Couldn't open file " << argv[1] << endl;
    return 2;
  }

  q2(fileStream);

  return 0;
}

void q1(fstream &fileStream) {
  vector<int> left;
  vector<int> right;

  for (string line; getline(fileStream, line);) {
    stringstream ss(line);
    int l, r;

    ss >> l;
    ss >> r;

    left.push_back(l);
    right.push_back(r);
  }

  ranges::sort(left);
  ranges::sort(right);

  auto res =
      ranges::views::zip(left, right) | ranges::views::transform([](auto it) {
        return abs(get<0>(it) - get<1>(it));
      });

  cout << ranges::fold_left(res, 0, plus<int>{}) << endl;
}

void q2(fstream &fileStream) {
  vector<int> left;
  unordered_map<int, int> freq;

  for (string line; getline(fileStream, line);) {
    stringstream ss(line);
    int l, r;

    ss >> l;
    ss >> r;

    left.push_back(l);
    freq[r]++;
  }

  auto similarities =
      ranges::views::transform(left, [&](int l) { return l * freq[l]; });
  auto result = ranges::fold_left(similarities, 0, plus<int>{});

  cout << result << endl;
}
