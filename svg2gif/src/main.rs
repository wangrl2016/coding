use std::fs::File;
use crate::app::render;
use std::io::Read;
use crate::context::{SvgContext, RenderContext};

mod app;
mod learn;
mod context;

const FPS: u32 = 24;

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

// 删除base64格式中gif中间的空行
fn delete_gif_newline(s: &String) -> String {
    // 去掉gif中的\n符号
    let mut res_str: String = String::new();        // 构建新的字符串传递到渲染函数中
    let symbol = "data:image/gif;base64";      // 查找到的gif标识
    let mut find_symbol = false;
    let mut in_symbol = false;

    for (i, c) in s.bytes().enumerate() {
        let ch = c as char;
        match s.get(i..(i + symbol.len())) {
            Some(str) => {
                if str == symbol {
                    find_symbol = true;
                }
                if find_symbol {
                    if ch == ',' || ch == '"' {
                        in_symbol = !in_symbol;
                        if in_symbol == false {
                            find_symbol = false;
                        }
                    }
                }
                if in_symbol {
                    if ch != '\n' {
                        res_str.push(ch);
                    }
                } else {
                    res_str.push(ch);
                }
            }
            _ => {
                res_str.push(ch);
            }
        }
    }
    res_str
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let mut args = pico_args::Arguments::from_env();
    let mut args = Args {
        help: args.contains(["-h", "--help"]),
        version: args.contains(["-v", "--version"]),
        input: args.opt_value_from_str(["-i", "--input"])?
            .unwrap_or(String::new()),
        output: args.opt_value_from_str(["-o", "--output"])?
            .unwrap_or(String::new()),
        format: args.opt_value_from_str(["-f", "--format"])?
            .unwrap_or(String::from("png")),
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
        // 需要正确处理../的情况
        let index = args.input.rfind(".").unwrap();
        if index > 0 {
            args.output = String::from(&args.input[0..index])
                + "." + args.format.as_str();
        } else {
            println!("Error parse input file name");
            std::process::exit(1);
        }
    }

    println!("Input filename: {}", args.input);
    println!("Output filename: {}", args.output);

    let mut file = File::open(args.input.as_str())
        .expect("Input file open failed");
    let mut source = Vec::<u8>::new();
    file.read_to_end(&mut source).unwrap();
    let svg_str = delete_gif_newline(&String::from_utf8(source).unwrap());

    let mut svg_context = SvgContext::new(&svg_str).unwrap();

    let mut render_context = RenderContext::new(&args.output, &args.format, FPS).unwrap();

    render(&svg_context, &render_context);

    Ok(())
}
