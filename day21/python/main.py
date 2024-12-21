import sys
from functools import lru_cache

door_keypad = [
    ['7', '8', '9'],
    ['4', '5', '6'],
    ['1', '2', '3'],
    [None, '0', 'A'],
]

dpad = [
    [None, '^', 'A'],
    ['<', 'v', '>'],
]

directions = {
    '^': (-1, 0),
    'v': (1, 0),
    '<': (0, -1),
    '>': (0, 1),
}

commands = [x.replace("\n", "") for x in open(sys.argv[1] if len(sys.argv) > 1 else '../example.txt', 'r').readlines()]

dp = {}
def get_paths(pos, target, seen, keypad_id, ind = 0):
    keypad = dpad if keypad_id == 1 else door_keypad

    if not (0 <= pos[0] < len(keypad) and 0 <= pos[1] < len(keypad[0])):
        return []

    if (pos, target, keypad_id, ind) in dp:
        return dp[(pos, target, keypad_id, ind)]

    if pos in seen:
        return []

    if keypad[pos[0]][pos[1]] is None:
        return []

    if keypad[pos[0]][pos[1]] == target:
        return ["A"]

    seen.add(pos)

    min_paths = []

    for s, (di, dj) in directions.items():
        paths = get_paths((pos[0] + di, pos[1] + dj), target, seen, keypad_id, ind + 1)
        if len(paths) > 0:
            min_paths.extend([s + x for x in paths])

    if len(min_paths) > 0:
        threshold = min([len(x) for x in min_paths])
        min_paths = [x for x in min_paths if len(x) == threshold]


    seen.remove(pos)

    dp[(pos, target, keypad_id, ind)] = min_paths
    return min_paths

def get_pos(keypad, target) -> tuple[int, int]:
    return [(i, r.index(target)) for i, r in enumerate(keypad) if target in r][0]

def get_numeric(command):
    return int("".join([x for x in command if x.isdigit()]))

@lru_cache
def get_expanded_length_after_n(command: str, n: int) -> int:
    if n == 0:
        return len(command)

    length = 0
    pos = (3, 2) if n == 26 else (0, 2)
    for c in command:
        paths = get_paths(pos, c, set(), 0 if n == 26 else 1)
        pos = get_pos(door_keypad if n == 26 else dpad, c)
        length += min([get_expanded_length_after_n(path, n-1) for path in paths])

    return length

print(sum([get_expanded_length_after_n(c, 26) * get_numeric(c) for c in commands]))
