from collections import defaultdict
from functools import lru_cache
import sys
filename = "../example.txt" if len(sys.argv) < 2 else sys.argv[1]
initial_secrets = [int(x.replace("\n", "")) for x in open(filename, 'r')]

def get_next_secret(secret) -> int:
    mod = 16777216
    secret = ((secret * 64) ^ secret) % mod
    secret = ((secret // 32) ^ secret) % mod
    secret = ((secret * 2048) ^ secret) % mod
    return secret


def part1(secret) -> int:
    for _ in range(2000):
        secret = get_next_secret(secret)
    return secret

def get_sequence_key(seq):
    return "".join([str(x) for x in seq])

dp1 = {}
def get_sequences(secret):
    initial = secret
    if initial in dp1:
        return dp1[initial]

    sequences = {}
    diffs = []

    for _ in range(2000):
        previous_price = secret % 10
        secret = get_next_secret(secret)
        current_price = secret % 10
        diff = current_price - previous_price
        diffs.append(diff)

        if len(diffs) >= 4:
            key = get_sequence_key(diffs[-4:])
            if key not in sequences:
                sequences[key] = current_price

    dp1[initial] = sequences
    return sequences

dp2 = {}
def get_value_for_sequence(secret, sequence):
    if (secret, sequence) in dp2:
        return dp2[(secret, sequence)]

    sequences = get_sequences(secret)
    if sequence in sequences:
        dp2[(secret, sequence)] = sequences[sequence]
        return sequences[sequence]

    dp2[(secret, sequence)] = 0
    return 0

def part2(secrets):
    best_value = 0
    best_sequence = None

    seen = set()

    for secret in secrets:
        seqs = get_sequences(secret)

        for seq in seqs:
            if seq in seen:
                continue
            seen.add(seq)

            value = 0
            for j, x in enumerate(secrets):
                if value + (9 * (len(secrets)-j)) <= best_value:
                    break
                value += get_value_for_sequence(x, seq)

            if value > best_value:
                best_value = value

    return best_value, best_sequence


print(sum([part1(x) for x in initial_secrets]))

for s in initial_secrets: get_sequences(s)
print(part2(initial_secrets))
