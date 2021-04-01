fn main() {
    if let Err(e) = process() {}
}

fn process() -> Result<(), String> {
    let args = match parse_args() {};
}

#[derive(Debug)]
struct CliArgs {
    help: bool,
    version: bool,

    width: Option<u32>,
    height: Option<u32>,
    zoom: Option<f32>,
    dpi: u32,

}

fn collect_args() -> Result<CliArgs, pico_args::Error> {

}

fn parse_args() -> Result<Args, String> {
    let mut args = collect_args().map_err(|e| e.to_string())?;
}
