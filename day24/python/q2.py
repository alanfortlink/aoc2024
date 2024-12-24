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

def print_tree(node, depth=0, seen = set(), spacer=" |  "):
    if node in seen:
        print(spacer * depth, node, "(seen)")
        return

    if node in initial_state:
        print(spacer * depth, initial_state[node], f"({node})", )
        return

    l, op, r = operations[node]
    print(spacer * depth, op, f"({node})")
    seen.add(node)
    print_tree(l, depth+1, seen)
    print_tree(r, depth+1, seen)

def to_dec(bits):
    return sum([2**i * b for i, b in enumerate(bits)])

x_bits = [v for k, v in sorted(initial_state.items()) if k.startswith('x')]
y_bits = [v for k, v in sorted(initial_state.items()) if k.startswith('y')]
z_bits = [value(z) for z in [x for x in sorted(operations) if x.startswith('z')]]

print(to_dec(x_bits))
print(to_dec(y_bits))
print(to_dec(z_bits))

print(to_dec(z_bits) == to_dec(x_bits) + to_dec(y_bits))

for z in sorted(operations):
    if z.startswith('z'):
        print_tree(z)

processed_z = set()
def is_valid_z(z, errors):
    # print("is_valid_z", z)
    if z not in operations: 
        errors.append(f"{z} not in operations")
        return False
    l, op, r = operations[z]
    if op != "XOR": 
        errors.append(f"{l} {op} {r} not a XOR")
        return False
    case1 = is_valid_sum(z, l, errors) and is_valid_carry(z, r, errors)
    case2 = is_valid_sum(z, r, errors) and is_valid_carry(z, l, errors)

    return case1 or case2

def is_valid_sum(z, v, errors):
    # print("is_valid_sum", z, v)
    if v not in operations: 
        errors.append(f"{v} not in operations")
        return False
    processed_z.add(v)
    l, op, r = operations[v]
    if op != "XOR": 
        errors.append(f"{l} {op} {r} not a XOR")
        return False
    if is_expected_p(z, 'x', l, errors) and is_expected_p(z, 'y', r, errors):
        return True
    if is_expected_p(z, 'y', l, errors) and is_expected_p(z, 'x', r, errors):
        return True

    return False

def is_valid_carry(z, v, errors):
    # print("is_valid_carry", z, v)
    if v not in operations: 
        errors.append(f"{v} is not an operation")
        return False
    l, op, r = operations[v]
    if op != "OR": 
        errors.append(f"{l} {op} {r} is not an OR")
        return False
    if is_valid_xy_and(z, l, errors) and is_valid_seen_and(z, r, errors):
        return True
    if is_valid_xy_and(z, r, errors) and is_valid_seen_and(z, l, errors):
        return True

    return False

def is_valid_xy_and(z, v, errors):
    if v not in operations: 
        errors.append(f"{v} is not an operation")
        return False
    l, op, r = operations[v]
    if op != "AND": 
        errors.append(f"{l} {op} {r} is not an AND")
        return False
    if l.startswith('x') and r.startswith('y'):
        return True
    if l.startswith('y') and r.startswith('x'):
        return True
    return False

def is_valid_seen_and(z, v, errors):
    # print("is_valid_seen_and", z, v)
    if v not in operations: 
        errors.append(f"{v} is not an operation")
        return False
    l, op, r = operations[v]
    if op != "AND": 
        errors.append(f"{l} {op} {r} is not an AND")
        return False
    return l in processed_z and r in processed_z

def is_expected_p(z, p, v, errors):
    if v.startswith(p) and v[1:] == z[1:]:
        return True

    errors.append(f"{z} {p} {v} didn't match")
    return False


for z in sorted(operations)[:-1]:
    if z.startswith('z'):
        errors = []
        if not is_valid_z(z, errors):
            print(f"{z} is invalid", errors)
