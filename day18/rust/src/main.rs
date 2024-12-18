use std::{
    collections::{HashSet, VecDeque},
    env::args,
    fs::File,
    io::{BufRead, BufReader},
};

type Coord = (i32, i32);
type State = (Coord, i32);

fn part1_rec(
    walls: &HashSet<&Coord>,
    grid_size: &Coord,
    pos: Coord,
    seen: &mut HashSet<Coord>,
) -> bool {
    !(pos.0 < 0 || pos.1 < 0 || pos.0 >= grid_size.0 || pos.1 >= grid_size.1)
        && !walls.contains(&pos)
        && (pos == (grid_size.0 - 1, grid_size.1 - 1)
            || (!seen.contains(&pos)
                && seen.insert(pos)
                && vec![(0, 1), (0, -1), (-1, 0), (1, 0)]
                    .into_iter()
                    .any(|d| part1_rec(walls, grid_size, (pos.0 + d.0, pos.1 + d.1), seen))))
}

fn part1(input: &Vec<Coord>, grid_size: Coord, bytes: usize) -> i32 {
    let mut q: VecDeque<State> = VecDeque::new();
    q.push_back(((0, 0), 0));

    let mut seen: HashSet<Coord> = HashSet::new();
    let walls = input.iter().take(bytes).collect::<HashSet<&Coord>>();

    while !q.is_empty() {
        let (coord, steps) = q.pop_front().unwrap();

        if coord == (grid_size.0 - 1, grid_size.1 - 1) {
            return steps;
        }

        if coord.0 < 0 || coord.0 >= grid_size.0 || coord.1 < 0 || coord.1 >= grid_size.1 {
            continue;
        }

        if walls.contains(&coord) {
            continue;
        }

        if seen.contains(&coord) {
            continue;
        }

        seen.insert(coord);

        for dir in [(0, -1), (0, 1), (1, 0), (-1, 0)] {
            q.push_back(((dir.0 + coord.0, dir.1 + coord.1), steps + 1));
        }
    }

    -1
}

fn part2(input: &Vec<Coord>, grid_size: Coord, bytes: usize) -> Option<usize> {
    (bytes..(input.len() + 1))
        .into_iter()
        .filter(|b| {
            let walls = input.iter().take(*b).collect::<HashSet<&Coord>>();
            let mut seen: HashSet<Coord> = HashSet::new();
            return !part1_rec(&walls, &grid_size, (0, 0), &mut seen);
        })
        .take(1)
        .next()
}

fn main() {
    let file_location = match args().len() >= 2 {
        true => args().nth(1).unwrap(),
        false => "../example.txt".to_string(),
    };

    let input: Vec<Coord> = BufReader::new(File::open(file_location).unwrap())
        .lines()
        .into_iter()
        .filter_map(Result::ok)
        .map(|line| {
            let tmp = line.split_once(",").unwrap();
            return (tmp.0.parse::<i32>().unwrap(), tmp.1.parse::<i32>().unwrap());
        })
        .collect::<Vec<_>>();

    let steps = part1(&input, (71, 71), 1024);
    println!("part1: {}", steps);

    let coord = part2(&input, (71, 71), 1024);
    println!("part2: {:?}", input[coord.unwrap() - 1]);
}
