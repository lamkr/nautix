mod db;
mod person;

extern crate rand;

use std::io;
use std::process::exit;
use crate::db::{Db, Identificable};
use crate::person::Person;

fn main() {
    let x = 5;
    let y = &x;

    println!("{}",&y);
    assert_eq!(5, x);
    //assert_eq!(5, y);
}

fn maior<'a>(x: &'a str, y: &'a str) -> &'a str {
    if x.len() > y.len() {
        x
    } else {
        y
    }
}

fn main2() {
    let mut db: Db<Person> = Db::new();
    println!("Syntax: <option> <id:usize>,<name:String>");
    loop {
        println!("[C]reate, [R]ead/[A]ll, [U]pdate, [D]elete, [L]ength, e[X]it: ");
        let mut args = String::new();
        match io::stdin().read_line(&mut args) {
            Ok(_) => {
                match args.chars().nth(0).unwrap_or('?') {
                    'C' => create(&mut db, args),
                    'R' => read(&db, args),
                    'U' => update(&mut db, args),
                    'D' => delete(&mut db, args),
                    'A' => read_all(&db),
                    'L' => length(&db),
                    'X' => exit(0),
                    _ => print!("# Invalid option: {}", args),
                }
            }
            Err(error) => println!("# Error: {}", error),
        }
    };
}

fn create(db: &mut Db<Person>, args: String) {
    if args.len() < 3 {
        println!("# Too few arguments. Syntax: C <name:String>");
        return;
    }
    let args: Vec<&str> = args[2..].split(',').collect();
    let name = args[0].trim().to_string();
    let entity = Person::new(name);
    db.create(entity);
}

fn read(db: &Db<Person>, args: String) {
    if args.len() < 3 {
        println!("# Too few arguments. Syntax: R <id:usize>");
        return;
    }
    let args: Vec<&str> = args[2..].split(',').collect();
    let id: usize = args[0].trim().parse().unwrap();
    if let Some(entity) = db.read(id) {
        println!("> {:?}", entity);
    }
}

fn update(db: &mut Db<Person>, args: String) {
    if args.len() < 5 {
        println!("# Too few arguments. Syntax: U <id:usize>,<name:String>");
        return;
    }
    let args: Vec<&str> = args[2..].split(',').collect();
    let id: usize = args[0].trim().parse().unwrap();
    let name = args[1].trim().to_string();
    let mut entity: Person = Person::new(name);
    entity.set_id(id);
    db.update(entity);
}

fn delete(db: &mut Db<Person>, args: String) {
    if args.len() < 3 {
        println!("# Too few arguments. Syntax: D <id:usize>");
        return;
    }
    let args: Vec<&str> = args[2..].split(',').collect();
    let id: usize = args[0].trim().parse().unwrap();
    if let Some(entity) = db.delete(id) {
        println!("> {:?}", entity);
    }
}

fn read_all(db: &Db<Person>) {
    for person in db.read_all() {
        println!("{:?}", person);
    }
}

fn length(db: &Db<Person>) {
    println!("{}", db.length())
}
