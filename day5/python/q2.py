import sys
lines = [x for x in open(sys.argv[1], 'r').readlines() if x != "\n"]
rules = [list(map(int, x.split("|"))) for x in lines if "|" in x]
updates = [list(map(int, x.replace("\n", "").split(","))) for x in lines if "|" not in x]
d = {rhs: set([x[0] for x in rules if x[1] == rhs]) for (_, rhs) in rules}

valids = []
for u in updates:
    r = []
    av = [x for x in u]
    is_correct = True
    while len(av) > 0:
        num = av.pop(0)
        if len(d[num].intersection(set(av))) == 0:
            r.append(num)
        else:
            is_correct = False
            av.append(num)
    if not is_correct:
        valids.append(r)

c = sum([x[len(x) // 2] for x in valids])
print(c)
