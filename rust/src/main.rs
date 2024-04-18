#![allow(dead_code)]
#![allow(unused)]

use std::{cell::RefCell, collections::HashMap, rc::Rc};
mod advance;
mod utils;

#[derive(Clone)]
pub struct Person {
    name: String,
    age: i32,
    children: HashMap<String, String>,
}

pub struct Home {
    owner: Option<Rc<RefCell<Person>>>,
}

impl Home {
    pub fn new(owner: Rc<RefCell<Person>>) -> Home {
        Home { owner: Some(owner) }
    }

    pub fn get_owner(&self) -> Person {
        self.owner.as_ref().unwrap().borrow().clone()
    }
}

fn main() {
    // crate::utils::run::run();
}
