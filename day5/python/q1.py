import sys
from collections import defaultdict
lines = [x for x in open(sys.argv[1], 'r').readlines() if x != "\n"]
rules = [list(map(int, x.split("|"))) for x in lines if "|" in x]
updates = [list(map(int, x.replace("\n", "").split(","))) for x in lines if "|" not in x]

d = defaultdict(set)
for (lhs, rhs) in rules:
    d[int(rhs)].add(int(lhs))

valids = []
for u in updates:
    prohibited = set()
    for v in u:
        if v in prohibited:
            break
        for p in d[v]:
            prohibited.add(p)
    else:
        valids.append(u)

c = sum([x[len(x) // 2] for x in valids])
print(c)
