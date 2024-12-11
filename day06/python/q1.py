import sys
content = open(sys.argv[1]).readlines()

DIRS = [(-1, 0), (0, 1), (1, 0), (0, -1)]


def next_dir(d):
    return DIRS[(DIRS.index(d) + 1) % len(DIRS)]


p = None
for i in range(len(content)):
    for j in range(len(content[0])):
        if content[i][j] == '^':
            p = (i, j)
            break


def is_inside(content, p):
    if p[0] < 0 or p[0] >= len(content):
        return False
    if p[1] < 0 or p[1] >= len(content[p[0]]):
        return False
    return True


def walk(content, p, d):
    visited = set()
    while is_inside(content, p):
        visited.add(p)
        nxt = (p[0] + d[0], p[1] + d[1])
        if is_inside(content, nxt):
            if content[nxt[0]][nxt[1]] == '#':
                d = next_dir(d)
                p = (p[0] + d[0], p[1] + d[1])
            else:
                p = nxt
        else:
            p = nxt

    return len(visited)


c = walk(content, p, DIRS[0])
print(c)
