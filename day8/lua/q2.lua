local grid = {}

local in_bounds = function(i, j)
  return i >= 1 and i <= #grid and j >= 1 and j <= #grid[i]
end

for line in io.lines(arg[1]) do
  local row = {}
  for i = 1, #line do
    row[i] = line:sub(i, i)
  end
  grid[#grid + 1] = row
end

local antennas = {}

for i, r in ipairs(grid) do
  for j = 1, #r do
    local v = r[j]
    if v ~= '.' then
      if not antennas[v] then
        antennas[v] = {}
      end

      antennas[v][{ i, j }] = true
    end
  end
end

local antinodes = {}

for _, k in pairs(antennas) do
  for p1, _ in pairs(k) do
    for p2, _ in pairs(k) do
      if p1 == p2 then
        goto continue
      end

      local p1top2 = { p2[1] - p1[1], p2[2] - p1[2] }
      local i = p1[1]
      local j = p1[2]

      while in_bounds(i, j) do
        local str_key = string.format("%d,%d", i, j)
        antinodes[str_key] = true
        i = i + p1top2[1]
        j = j + p1top2[2]
      end

      ::continue::
    end
  end
end

local count = 0
for _, _ in pairs(antinodes) do
  count = count + 1
end

print(count)
