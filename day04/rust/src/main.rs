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

    if i < 0 || j < 0 || i >= matrix.len() as i32 || j >= matrix.first().unwrap().len() as i32 {
        return false;
    }

    let value = matrix
        .get(i as usize)
        .and_then(|row| row.get(j as usize))
        .unwrap();

    return *value == needle.chars().nth(index).unwrap()
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

    matrix
        .iter()
        .enumerate()
        .map(|(i, r)| {
            (0..r.len())
                .map(move |j| {
                    directions
                        .iter()
                        .filter(|dir| search(matrix, "XMAS", 0, i as i32, j as i32, **dir))
                        .count()
                })
                .sum::<usize>()
        })
        .sum::<usize>()
}

fn q2(matrix: &Vec<Vec<char>>) -> usize {
    matrix
        .iter()
        .enumerate()
        .map(|(i, r)| {
            (0..r.len())
                .filter(move |j| {
                    (search(matrix, "MAS", 0, i as i32 + 1, *j as i32 - 1, (-1, 1))
                        || search(matrix, "MAS", 0, i as i32 - 1, *j as i32 + 1, (1, -1)))
                        && (search(matrix, "MAS", 0, i as i32 - 1, *j as i32 - 1, (1, 1))
                            || search(matrix, "MAS", 0, i as i32 + 1, *j as i32 + 1, (-1, -1)))
                })
                .count()
        })
        .sum::<usize>()
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
