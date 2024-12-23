import sys
from collections import defaultdict
import itertools

filename = sys.argv[1] if len(sys.argv) > 1 else "../example.txt"
pairs = [tuple(x.replace("\n", "").split("-")) for x in open(filename, 'r').readlines()]

V = list(set(itertools.chain.from_iterable(pairs)))
E = defaultdict(set)

for u, v in pairs:
    E[u].add(v)
    E[v].add(u)

def part1():
    triples = set()
    v_with_t = [v for v in V if v.startswith('t')]
    for v in v_with_t:
        for u in E[v]:
            for w in E[u]:
                if u != v and v != w and u != w:
                    if w in E[v]:
                        triples.add(tuple(sorted((v, u, w))))
    return triples

seen = set()
ccs = set()
def walk(v, cc):
    key = tuple(sorted(cc))
    if key in ccs:
        return

    ccs.add(key)

    for u in E[v]:
        if u in cc: continue
        if len([w for w in cc if u not in E[w]]) != 0: continue
        walk(u, {*cc, u})


def part2():
    for v in V:
        walk(v, {v})
    return max(ccs, key=len)


triples = part1()
print(len(triples))

print(",".join(sorted(part2())))
