use crate::utils::util::*;

fn add(a: &i32, b: &i32) -> i32 {
    a + b
}

fn deref_demo() {
    let a = Box::new(Box::new(Box::new(9)));
    let b = 10;
    println!("只有引用类型才会触发deref");
    println!("ans = {}", add(&a, &b))
}

pub fn exec() {
    deref_demo();
}
