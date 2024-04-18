pub fn show_chapter(str: &str) {
    let mut format_line = String::new();

    let chinese_count = str.chars().filter(|c| c.is_ascii() == false).count();
    let english_count = str.chars().filter(|c| c.is_ascii()).count();
    let len = chinese_count * 2 + english_count;

    for _ in 0..(len + 2) {
        format_line.push_str("═");
    }
    println!("╔{}╗", format_line);
    println!("║ {} ║", str);
    println!("╚{}╝", format_line);
}
pub fn show_title(str: &str) {
    let mut format_line = String::new();

    let chinese_count = str.chars().filter(|c| c.is_ascii() == false).count();
    let english_count = str.chars().filter(|c| c.is_ascii()).count();
    let len = chinese_count * 2 + english_count;

    for _ in 0..(len + 2) {
        format_line.push_str("─");
    }
    println!("\t┌{}┐", format_line);
    println!("\t│ {} │", str);
    println!("\t└{}┘", format_line);
}
