import sys

grid = [[int(c) for c in x.rstrip()]
        for x in open(sys.argv[1], 'r').readlines()]

directions = [(0, 1), (0, -1), (1, 0), (-1, 0)]


def walk(i, j, reached):
    if grid[i][j] == 9:
        reached.add((i, j))
        return

    for di, dj in directions:
        ni, nj = i + di, j + dj
        if ni < 0 or ni >= len(grid) or nj < 0 or nj >= len(grid[i]):
            continue
        if grid[i][j]+1 != grid[ni][nj]:
            continue

        walk(ni, nj, reached)


reacheable = 0
for i, r in enumerate(grid):
    for j, c in enumerate(r):
        if grid[i][j] == 0:
            reached = set()
            walk(i, j, reached)
            reacheable += len(reached)

print(reacheable)
