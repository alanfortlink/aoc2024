import sys
from collections import defaultdict
content = open(sys.argv[1], 'r').read()
keys_and_locks = [[y.replace("\n", "") for y in x.split("\n") if y != ''] for x in content.split("\n\n")]

def is_lock(kl):
    return all(x == '#' for x in kl[0])

def parse(kl):
    klc = [0] * len(kl[0])
    valid_kl = kl[1:] if is_lock(kl) else kl[:-1]
    for _, r in enumerate(valid_kl):
        for j, v in enumerate(r):
            if v == '#':
                klc[j] += 1
    return klc

def fits(lock, key):
    return all((x+y <= 5) for  x, y in zip(lock, key))

locks = [parse(x) for x in keys_and_locks if is_lock(x)]
keys = [parse(x) for x in keys_and_locks if not is_lock(x)]

count = 0

for l in locks:
    for k in keys:
        if fits(l, k):
            count += 1

print(count)
