use std::path;

fn main() {
    if let Err(e) = process() {
        eprintln!("Error:{}", e);
        std::process::exit(1);
    }
}

fn process() -> Result<(), String> {
    let args = match parse_args() {
        Ok(args) => args,
        Err(e) => {
            println!("{}", HELP);
            return Err(format!("{}", e));
        }
    };

    Ok(())
}

struct Args {
    in_svg: path::PathBuf,
    out_png: Option<path::PathBuf>,
    query_all: bool,
    export_id: Option<String>,
    dump: Option<path::PathBuf>,
    perf: bool,
    quiet: bool,
    usvg: usvg::Options,
    fit_to: usvg::FitTo,
    background: Option<usvg::Color>,
}

fn parse_args() -> Result<Args, String> {
    let mut args = collect_args().map_err(|e| e.to_string())?;

    Ok(Args {
        in_svg: Default::default(),
        out_png: None,
        query_all: false,
        export_id: None,
        dump: None,
        perf: false,
        quiet: false,
        usvg: Default::default(),
        fit_to: usvg::FitTo::Original,
        background: None,
    })
}

#[derive(Debug)]
struct CliArgs {
    help: bool,
    version: bool,
    width: Option<u32>,
    height: Option<u32>,
    zoom: Option<f32>,
    dpi: u32,
    background: Option<usvg::Color>,
}

fn collect_args() -> Result<CliArgs, pico_args::Error> {
    let mut input = pico_args::Arguments::from_env();
    Ok(CliArgs {
        help: input.contains(["-h", "--help"]),
        version: input.contains(["-v", "--version"]),
        width: None,
        height: None,
        zoom: None,
        dpi: 0,
        background: None,
    })
}

const HELP: &str = "\
svg is an SVG rendering application.

USAGE:
    svg [OPTIONS] <in-svg> <out-image>

    svg in.svg out.png
    svg --query-all in.svg

OPTIONS:
    -h, --help                  Prints this help
    -v, --version               Prints version
    -w, --width LENGTH          Sets the width in pixels
    -h, --height LENGTH         Sets the height in pixels
    -z, --zoom FACTOR           Zooms the resolution
        --dpi DPI               Sets the resolution
                                [default:96] [possible values: 10..4000]
    --background COLOR          Sets the background color
                                Examples: red, #fff, #fff000
";
