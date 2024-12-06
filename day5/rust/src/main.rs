use std::{
    fs::File,
    io::{BufRead, BufReader},
};

fn main() {
    let file_location = std::env::args().into_iter().nth(1).unwrap();
    let empty = String::from("");
    let lines = BufReader::new(File::open(file_location).unwrap())
        .lines()
        .filter_map(|l| match l {
            Ok(content) => match content.as_str() {
                "" => Some(content),
                _ => None,
            },
            _ => None,
        })
        .collect::<Vec<_>>();

    let rules = lines
        .iter()
        .filter_map(|l| match l.contains("|") {
            true => Some(l.split_once("|").unwrap()),
            _ => None,
        })
        .map(|(lhs, rhs)| (lhs.parse::<i32>().unwrap(), rhs.parse::<i32>().unwrap()));

    let updates = lines.iter().filter_map(|l| match l.contains(",") {
        true => Some(l.split(",").map(|v| v.parse::<i32>().unwrap())),
        _ => None,
    });
}
