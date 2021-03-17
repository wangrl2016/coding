mod lib;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let mut args = pico_args::Arguments::from_env();

    println!("Hello, world!");

    return Ok(());
}
