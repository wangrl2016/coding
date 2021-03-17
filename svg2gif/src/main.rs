use std::fs::File;

struct Args {
    help: bool,
    version: bool,
    input: String,
    output: String,
    format: String,
}

const HELP: &str = "svg2gif is an svg rendering application

USAGE:
    svg2gif -i input_svg -o output_img -f format

OPTIONS:
    -h, --help          Prints this help
    -v, --version       Prints version
    -i, --input         Input svg file path
    -o, --output        Output img file path
    -f, --format        Output format (png|jpg|jpeg|gif)

ARGS:
    input_svg           Input svg file
    output_img          Output img file
    format              File format
";

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let mut args = pico_args::Arguments::from_env();
    let mut args = Args {
        help: args.contains(["-h", "--help"]),
        version: args.contains(["-v", "--version"]),
        input: args.opt_value_from_str(["-i", "--input"])?.unwrap_or(String::new()),
        output: args.opt_value_from_str(["-o", "--output"])?.unwrap_or(String::new()),
        format: args.opt_value_from_str(["-f", "--format"])?.unwrap_or(String::from("png")),
    };

    if args.help {
        print!("{}", HELP);
        std::process::exit(0);
    }

    if args.version {
        println!("{}", env!("CARGO_PKG_VERSION"));
        std::process::exit(0);
    }

    if args.input.is_empty() {
        println!("Input is empty");
        std::process::exit(1);
    } else if args.output.is_empty() {
        // 替换输入文件的后缀转化为输出文件名
        let str_vec: Vec<&str> = args.input.split(".").collect();
        if str_vec.len() > 1 {
            args.output = String::from(str_vec[0]) + "." + args.format.as_str();
        } else {
            println!("Error parse input file name");
            std::process::exit(1);
        }
    }

    println!("Input filename: {}", args.input);
    println!("Output filename: {}", args.output);

    // let mut file = File::open(args.input.as_str()).expect("Input file open failed");


    return Ok(());
}
