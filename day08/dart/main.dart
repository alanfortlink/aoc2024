import 'dart:collection';
import 'dart:io';

void main(List<String> args) async {
  final file = File(args.first);
  final grid = await file.readAsLines();

  final antennas = HashMap<String, List<(int, int)>>();
  for (int i = 0; i < grid.length; i++) {
    for (int j = 0; j < grid[i].length; j++) {
      String c = grid[i][j];
      if (c != '.') {
        if (!antennas.containsKey(c)) antennas[c] = [];
        antennas[c]!.add((i, j));
      }
    }
  }

  final inBounds = (int x, int y) {
    return x >= 0 && x < grid.length && y >= 0 && y < grid[x].length;
  };

  final antinodes = HashSet<(int, int)>();
  for (final item in antennas.entries) {
    final pos = item.value;
    for (int i = 0; i < pos.length; i++) {
      for (int j = 0; j < pos.length; j++) {
        if (i == j) {
          continue;
        }

        final itoj = (pos[j].$1 - pos[i].$1, pos[j].$2 - pos[i].$2);
        int x = pos[i].$1;
        int y = pos[i].$2;

        while (inBounds(x, y)) {
          antinodes.add((x, y));
          x += itoj.$1;
          y += itoj.$2;
        }
      }
    }
  }

  print(antinodes.length);
}
