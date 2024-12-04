use std::{
    collections::HashSet,
    fs::File,
    io::{BufRead, BufReader},
};

fn search(
    matrix: &Vec<Vec<char>>,
    needle: &str,
    index: usize,
    i: i32,
    j: i32,
    dir: (i32, i32),
) -> bool {
    if index == needle.len() {
        return true;
    }

    if i < 0 || j < 0 {
        return false;
    }

    let row = matrix.get(i as usize);
    if row.is_none() {
        return false;
    }

    let value = row.unwrap().get(j as usize);
    if value.is_none() {
        return false;
    }

    return *value.unwrap() == needle.chars().nth(index).unwrap()
        && search(matrix, needle, index + 1, i + dir.0, j + dir.1, dir);
}

fn q1(matrix: &Vec<Vec<char>>) -> usize {
    let directions = [
        (0, 1),
        (0, -1),
        (1, 0),
        (-1, 0),
        (1, 1),
        (1, -1),
        (-1, 1),
        (-1, -1),
    ];
    let mut C: HashSet<(i32, i32, (i32, i32))> = HashSet::new();

    for (i, row) in matrix.iter().enumerate() {
        for j in 0..row.len() {
            for dir in directions {
                if search(&matrix, "XMAS", 0, i as i32, j as i32, dir) {
                    C.insert((i as i32, j as i32, dir));
                }
            }
        }
    }

    C.len()
}

fn q2(matrix: &Vec<Vec<char>>) -> usize {
    let mut c: HashSet<(usize, usize)> = HashSet::new();

    for (i, row) in matrix.iter().enumerate() {
        for j in 0..row.len() {
            let is_first_mas = search(matrix, "MAS", 0, i as i32 + 1, j as i32 - 1, (-1, 1))
                || search(matrix, "MAS", 0, i as i32 - 1, j as i32 + 1, (1, -1));

            let is_second_mas = search(matrix, "MAS", 0, i as i32 - 1, j as i32 - 1, (1, 1))
                || search(matrix, "MAS", 0, i as i32 + 1, j as i32 + 1, (-1, -1));

            if is_first_mas && is_second_mas {
                c.insert((i, j));
            }
        }
    }

    c.len()
}

fn main() {
    let matrix = BufReader::new(File::open("../input.txt").unwrap())
        .lines()
        .filter_map(|x| match x {
            Ok(v) => Some(v.chars().collect::<Vec<char>>()),
            _ => None,
        })
        .collect::<Vec<_>>();

    println!("{}", q2(&matrix));
}
