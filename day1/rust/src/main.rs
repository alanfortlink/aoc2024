use std::{
    collections::HashMap,
    fs::File,
    io::{BufRead, BufReader},
    process::exit,
};

fn main() {
    let mut args = std::env::args();
    let program_name = args.next().unwrap();

    if args.len() != 1 {
        println!("Usage: {} <file_location>", program_name);
        exit(1);
    }

    let file_location = args.next().unwrap();
    let file = File::open(&file_location);

    if file.is_err() {
        println!("Couldn't open file {}", file_location);
        exit(2);
    }

    let reader = BufReader::new(file.unwrap());

    q2(reader);
}

fn q1(reader: BufReader<File>) {
    let mut left: Vec<i32> = Vec::new();
    let mut right: Vec<i32> = Vec::new();

    for line in reader.lines() {
        if let Ok(content) = line {
            let mut split = content.split_whitespace().map(|v| v.parse().unwrap());

            left.push(split.next().unwrap());
            right.push(split.next().unwrap());
        }
    }

    left.sort();
    right.sort();

    let diff: i32 = left
        .into_iter()
        .zip(right)
        .map(|it| (it.0 - it.1).abs())
        .sum();

    println!("{}", diff);
}

fn q2(reader: BufReader<File>) {
    let mut left: Vec<i32> = Vec::new();
    let mut freq: HashMap<i32, i32> = HashMap::new();

    for line in reader.lines() {
        if let Ok(content) = line {
            let mut split = content.split_whitespace().map(|v| v.parse().unwrap());

            left.push(split.next().unwrap());
            let right = split.next().unwrap();

            freq.insert(right, freq.get(&right).unwrap_or(&0) + 1);
        }
    }

    let diff: i32 = left
        .into_iter()
        .map(|it| (it * freq.get(&it).unwrap_or(&0)))
        .sum();

    println!("{}", diff);
}
