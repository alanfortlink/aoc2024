import sys
content = [list(x) for x in open(sys.argv[1]).readlines()]

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


def is_cycle(content, p, d):
    visited = []
    while is_inside(content, p):
        if (p, d) in visited:
            return True
        visited.append((p, d))
        nxt = (p[0] + d[0], p[1] + d[1])
        if is_inside(content, nxt):
            if content[nxt[0]][nxt[1]] == '#':
                d = next_dir(d)
                p = (p[0] + d[0], p[1] + d[1])
            else:
                p = nxt
        else:
            p = nxt

    return False


c = 0

for i in range(len(content)):
    print(f"row {i}/{len(content)}")
    for j in range(len(content[i])):
        if content[i][j] == '#':
            continue
        content[i][j] = '#'
        if is_cycle(content, p, DIRS[0]):
            c += 1
        content[i][j] = '.'

print(c)
