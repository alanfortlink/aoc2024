import sys
from collections import deque

def q2(line):
    target, nums = line.split(":")
    target = int(target)
    nums = [int(x) for x in nums.strip().split(" ")]
    n = len(nums)

    q = deque()
    q.append((nums[0], 0))
    while q:
        (curr, idx) = q.popleft()
        if curr == target and idx == n-1:
            return target

        if curr > target or idx == n-1:
            continue

        cand = nums[idx+1]
        q.append((curr + cand, idx+1))
        q.append((curr * cand, idx+1))
        q.append((int(str(curr) + str(cand)), idx+1))

    return 0

print(sum([q2(x) for x in open(sys.argv[1], 'r').readlines()]))
