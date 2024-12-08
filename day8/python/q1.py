import sys
from collections import defaultdict
from math import sqrt

grid = [x.strip().replace("\n", "") for x in open(sys.argv[1], 'r').readlines()]
antennas = defaultdict(set)

for (i, r) in enumerate(grid):
    for (j, c) in enumerate(r):
        if c != '.':
            antennas[c].add((i, j))

def in_bounds(x, y):
    return x >= 0 and x < len(grid) and y >= 0 and y < len(grid[x])

def dist(x1, y1, x2, y2):
    return sqrt((x1 - x2) ** 2 + (y1 - y2) ** 2)

antinodes = set()
for (frequency, locations) in antennas.items():
    for (i, (x1, y1)) in enumerate(locations):
        for (j, (x2, y2)) in enumerate(locations):
            if i == j: continue
            itoj = (x2 - x1, y2 - y1)

            x, y = x1, y1
            while in_bounds(x, y):
                dorig = dist(x, y, x1, y1)
                ddest = dist(x, y, x2, y2)

                if dorig == 2 * ddest:
                    antinodes.add((x, y))

                x, y = x + itoj[0], y + itoj[1]

print(len(antinodes))
