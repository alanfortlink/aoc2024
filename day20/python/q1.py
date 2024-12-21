import sys
from collections import defaultdict


sys.setrecursionlimit(10000)


grid = [list(x.replace("\n", "")) for x in open(sys.argv[1], 'r').readlines()]
start, grid_exit = None, None
directions = ((0, 1), (0, -1), (1, 0), (-1, 0))

for i in range(len(grid)):
    for j in range(len(grid[i])):
        if grid[i][j] == 'S':
            start = (i, j)
        if grid[i][j] == 'E':
            grid_exit = (i, j)


solutions = defaultdict(set)


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

    for di, dj in directions:
        tail = search(i + di, j + dj, seen)
        if tail is not None:
            return [(i, j)] + tail
    return None


path = search(start[0], start[1], set())

per_gain = defaultdict(int)

for index, (i, j) in enumerate(path):
    for di, dj in directions:
        ni, nj = i + 2 * di, j + 2 * dj

        try:
            pos = path.index((ni, nj))
            gain = (pos - index - 2)
            if gain > 0:
                per_gain[gain] += 1
        except:
            pass

for _, k, v in sorted([(k, k, v) for k, v in per_gain.items()]):
    print(f"There are {v} cheats that save {k} picoseconds")

print(sum([v for k, v in per_gain.items() if k >= 100]))
