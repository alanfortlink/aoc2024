import sys
from functools import cmp_to_key

lines = [x for x in open(sys.argv[1], 'r').readlines() if x != "\n"]
rules = [list(map(int, x.split("|"))) for x in lines if "|" in x]
updates = [list(map(int, x.replace("\n", "").split(","))) for x in lines if "|" not in x]
d = {rhs: set([x[0] for x in rules if x[1] == rhs]) for (_, rhs) in rules}

invalids = []
for u in updates:
    prohibited = set()
    for v in u:
        if v in prohibited:
            invalids.append(u)
            break
        for p in d[v]:
            prohibited.add(p)


def compare(lhs, rhs):
    if lhs in d[rhs]:
        return True
    if rhs in d[lhs]:
        return False
    return lhs < rhs


invalids = [sorted(x, key=cmp_to_key(compare)) for x in invalids]
c = sum([x[len(x) // 2] for x in invalids])
print(c)
