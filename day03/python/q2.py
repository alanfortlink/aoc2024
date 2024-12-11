import sys
import re

content = open(sys.argv[1], 'r').read()
regexes = [r"mul\(([0-9][0-9]*),([0-9][0-9]*)\)", r"don't\(\)", r"do\(\)"]
enabled = True
total = 0
for m in re.compile("|".join(regexes)).finditer(content):
    enabled = (enabled or "do()" in m.group()) and ("don't" not in m.group())
    if "mul" in m.group() and enabled:
        total += int(m.group(1)) * int(m.group(2))
print(total)
