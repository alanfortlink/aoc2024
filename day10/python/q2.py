import sys

grid = [[int(c) for c in x.rstrip()]
        for x in open(sys.argv[1], 'r').readlines()]

directions = [(0, 1), (0, -1), (1, 0), (-1, 0)]


def walk(i, j):
    if grid[i][j] == 9:
        return 1

    c = 0
    for di, dj in directions:
        ni, nj = i + di, j + dj
        if ni < 0 or ni >= len(grid) or nj < 0 or nj >= len(grid[i]):
            continue
        if grid[i][j]+1 != grid[ni][nj]:
            continue

        c += walk(ni, nj)
    return c


reacheable = 0
for i, r in enumerate(grid):
    for j, c in enumerate(r):
        if grid[i][j] == 0:
            reacheable += walk(i, j)

print(reacheable)
