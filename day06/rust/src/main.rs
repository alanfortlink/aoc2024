use std::{
    collections::HashSet,
    fs::File,
    io::{BufRead, BufReader},
    usize,
};

const N: (i32, i32) = (-1, 0);
const E: (i32, i32) = (0, 1);
const S: (i32, i32) = (1, 0);
const W: (i32, i32) = (0, -1);

fn next(d: (i32, i32)) -> (i32, i32) {
    match d {
        N => E,
        E => S,
        S => W,
        W => N,
        _ => N,
    }
}

fn is_inside(matrix: &Vec<Vec<char>>, p: (i32, i32)) -> bool {
    p.0 >= 0 && p.0 < matrix.len() as i32 && p.1 >= 0 && p.1 < matrix[p.0 as usize].len() as i32
}

fn is_cycle(matrix: &Vec<Vec<char>>, p: (i32, i32), d: (i32, i32)) -> bool {
    let mut p = p;
    let mut d = d;

    let mut visited: HashSet<((i32, i32), (i32, i32))> = HashSet::new();

    while is_inside(matrix, p) {
        if visited.contains(&(p, d)) {
            return true;
        }
        visited.insert((p, d));
        let nxt = (p.0 + d.0, p.1 + d.1);
        if is_inside(matrix, nxt) {
            let value = matrix[nxt.0 as usize][nxt.1 as usize];

            if value == '#' {
                d = next(d);
            } else {
                p = nxt;
            }
        } else {
            return false;
        }
    }

    return false;
}

fn main() {
    let file_location = std::env::args().into_iter().nth(1).unwrap();
    let mut matrix = BufReader::new(File::open(file_location).unwrap())
        .lines()
        .filter_map(|l| match l {
            Ok(c) => Some(c.chars().collect::<Vec<_>>()),
            _ => None,
        })
        .collect::<Vec<_>>();

    let mut p: (i32, i32) = (0, 0);
    for i in 0..matrix.len() {
        for j in 0..matrix.get(i).unwrap().len() {
            let value = matrix
                .get(i as usize)
                .and_then(|row| row.get(j as usize))
                .unwrap();

            if *value == '^' {
                p = (i as i32, j as i32);
                break;
            }
        }
    }

    let mut c = 0;

    for i in 0..matrix.len() {
        println!("Row {}/{}", i, matrix.len());
        for j in 0..matrix.get(i).unwrap().len() {
            if matrix[i][j] == '.' {
                matrix[i][j] = '#';
                if is_cycle(&matrix, p, N) {
                    c += 1;
                }
                matrix[i][j] = '.';
            }
        }
    }

    println!("{:?}", c);
}
