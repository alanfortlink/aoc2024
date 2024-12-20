#include <cstdint>
#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

int main(int argc, char **argv) {
  string file_location = argc >= 2 ? argv[1] : "../example.txt";
  fstream fs(file_location);

  auto is_free = [](int i) { return i % 2 != 0; };
  auto get_file_id = [](int i) { return ceil(i / 2); };

  vector<int> disk_map;
  for (char c; fs >> c; disk_map.push_back(c - '0'))
    ;

  int index = 0;
  uint64_t checksum = 0;

  auto add_to_checksum = [&](int length, int file_id) {
    while (length--) {
      checksum += index++ * file_id;
    }
  };

  auto q1 = [&]() {
    int b = 0;
    int e = disk_map.size() - 1;

    while (b <= e) {
      int &bv = disk_map[b];
      int &ev = disk_map[e];

      if (!is_free(b)) {
        add_to_checksum(bv, get_file_id(b));
        b++;
        continue;
      }

      if (is_free(e)) {
        e -= 2;
        continue;
      }

      if (bv == 0) {
        b++;
        continue;
      }

      if (ev == 0) {
        e -= 2;
        continue;
      }

      add_to_checksum(1, get_file_id(e));
      ev--;
      bv--;
    }
  };

  auto q2 = [&]() {
    int b = 0;
    int end = disk_map.size() - 1;

    for (int b = 0; b <= end; b++) {
      int &bv = disk_map[b];
      if (bv < 0) {
        add_to_checksum(-bv, 0);
        continue;
      }

      if (!is_free(b)) {
        add_to_checksum(abs(disk_map[b]), get_file_id(b));
        continue;
      }

      if (bv == 0) {
        continue;
      }

      for (int e = end; e >= b && bv > 0; e -= 2) {
        int &ev = disk_map[e];
        if (ev <= 0) {
          continue;
        }

        if (ev <= bv) {
          add_to_checksum(ev, get_file_id(e));
          bv -= ev;
          ev = -ev;
        }
      }

      add_to_checksum(bv, 0);
    }
  };

  q2();
  cout << checksum << endl;
}
