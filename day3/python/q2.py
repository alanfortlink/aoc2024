import sys
import re

content = open(sys.argv[1], 'r').read()
enabled = True

INF = len(content) + 10

events = []
regexes = [r"mul\([0-9][0-9]*,[0-9][0-9]*\)", r"don't\(\)", r"do\(\)"]

for r in regexes:
    items = [(item.span()[0], item.group())
             for item in re.compile(r).finditer(content)]
    events.extend(items)

events.sort(key=lambda x: x[0])
enabled = True
total = 0
for e in events:
    if "do()" in e[1]:
        enabled = True
        continue
    if "don't()" in e[1]:
        enabled = False
        continue

    if enabled:
        inner = e[1].replace("mul(", "").replace(")", "").split(",")
        lhs, rhs = int(inner[0]), int(inner[1])
        total += lhs * rhs

print(total)
