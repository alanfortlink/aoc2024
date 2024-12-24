import sys
filename = "../small_example.txt" if len(sys.argv[1]) < 2 else sys.argv[1]
content = open(filename, 'r').read()
initial_state, operations = content.split("\n\n")
initial_state = {l: int(r) for l, r in [s.split(": ") for s in initial_state.split("\n")]}
operations = [(tuple(l.strip().split(" ")), r.strip()) for l, r in [x.split("->") for x in operations.split("\n") if len(x) > 0]]
operations = {dst: op for op, dst in operations}

def value(output):
    if output in initial_state:
        return initial_state[output]

    l, op, r = operations[output]

    if op == "XOR":
        return value(l) ^ value(r)

    if op == "AND":
        return value(l) & value(r)

    if op == "OR":
        return value(l) | value(r)

    raise Exception("unexpected operator", op)

values = {z: value(z) for z in [x for x in operations if x.startswith('z')]}
res = sum([values[k] * pow(2, i) for i, k in enumerate(sorted(values.keys()))])
print(res)
