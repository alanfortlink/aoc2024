import sys

lines = [x.replace("\n", "") for x in open(sys.argv[1], 'r').readlines()]
available = set([x.strip() for x in lines[0].split(",")])

dp = {}

def solve(w):
    if w in dp:
        return dp[w]

    count = 0
    if w == "":
        return 1

    for p in available:
        if w.startswith(p):
            count += solve(w[len(p):])

    dp[w] = count
    return count

# q1
print(len([w for w in lines[2:] if solve(w) > 0]))

# q2
print(sum([solve(w) for w in lines[2:]]))
