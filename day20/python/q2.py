import sys
from collections import defaultdict

sys.setrecursionlimit(10000)

grid = [list(x.replace("\n", "")) for x in open(sys.argv[1], 'r').readlines()]
start = [[(i, j) for j, col in enumerate(row) if col == "S"][0]
         for i, row in enumerate(grid) if "S" in row][0]


def search(i, j, seen):
    if i < 0 or i >= len(grid) or j < 0 or j >= len(grid[i]):
        return None

    if (i, j) in seen:
        return None

    if grid[i][j] == '#':
        return None

    if grid[i][j] == 'E':
        return [(i, j)]

    seen.add((i, j))

    for di, dj in ((0, 1), (0, -1), (1, 0), (-1, 0)):
        tail = search(i + di, j + dj, seen)
        if tail is not None:
            return [(i, j)] + tail
    return None


path = search(start[0], start[1], set())
per_gain = defaultdict(int)

for index1, (i1, j1) in enumerate(path):
    for index2 in range(index1+1, len(path)):
        i2, j2 = path[index2]
        dist = abs(i2 - i1) + abs(j2 - j1)

        if dist <= 20:
            gain = index2 - index1 - dist
            if gain > 0:
                per_gain[gain] += 1

for _, k, v in sorted([(k, k, v) for k, v in per_gain.items()]):
    if k >= 100:
        print(f"There are {v} cheats that save {k} picoseconds")

print(sum([v for k, v in per_gain.items() if k >= 100]))
