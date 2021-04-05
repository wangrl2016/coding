use std::path;
use std::path::PathBuf;
use log::{Metadata, Record, Level};
use usvg::NodeExt;
use usvg::fontdb::Database;
use std::io::Write;

macro_rules! timed {
    ($args:expr, $name:expr, $task:expr) => {
        if $args.perf {
            let now = std::time::Instant::now();
            let res = $task;
            println!("{}: {:.2}ms", $name, now.elapsed().as_micros() as f64 / 1000.0);
            res
        } else {
            $task
        }
    };
}

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

    // Do not print warning during the ID querying.
    //
    // Some crates still can print to stdout/stderr, but we can't do anything about it.
    if !(args.query_all || args.quiet) {
        if let Ok(()) = log::set_logger(&LOGGER) {}
    }

    // Load file.
    let tree = timed!(args, "Preprocessing",
        usvg::Tree::from_file(&args.in_svg, &args.usvg).map_err(|e| e.to_string()))?;

    if args.query_all {
        return query_all(&tree);
    }

    // Dump before rendering in case of panic.
    if let Some(ref dump_path) = args.dump {
        dump_svg(&tree, dump_path)?;
    }

    let out_image = match args.out_image {
        Some(ref path) => path.clone(),
        None => return Ok(()),
    };

    // render
    render_svg(args, &tree, &out_image)
}

struct Args {
    in_svg: path::PathBuf,
    out_image: Option<path::PathBuf>,
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
    let args = collect_args().map_err(|e| e.to_string())?;

    if args.help {
        print!("{}", HELP);
        std::process::exit(0);
    }

    if args.version {
        println!("{}", env!("CARGO_PKG_VERSION"));
        std::process::exit(0);
    }

    if !args.query_all && args.output.is_none() {
        return Err(String::from("Output must be set"));
    }

    let in_svg = args.input.unwrap();
    let out_image = args.output;

    let dump = args.dump_svg.as_ref().map(|v| v.into());
    let export_id = args.export_id.as_ref().map(|v| v.to_string());

    // We don't have to keep named groups when we don't need them,
    // because it will slow down rendering.
    let keep_named_groups = args.query_all || export_id.is_some();

    let mut fit_to = usvg::FitTo::Original;
    if let Some(w) = args.width {
        fit_to = usvg::FitTo::Width(w);
    } else if let Some(h) = args.height {
        fit_to = usvg::FitTo::Height(h);
    } else if let Some(z) = args.zoom {
        fit_to = usvg::FitTo::Zoom(z);
    }

    let resource_dir = match args.resources_dir {
        Some(v) => Some(v),
        None => {
            // Get input file absolute directory.
            std::fs::canonicalize(&in_svg).ok()
                .and_then(|p| p.parent().map(|p| p.to_path_buf()))
        }
    };

    let usvg = usvg::Options {
        resources_dir: resource_dir,
        dpi: args.dpi as f64,
        font_family: "".to_string(),
        font_size: 0.0,
        languages: vec![],
        shape_rendering: Default::default(),
        text_rendering: Default::default(),
        image_rendering: Default::default(),
        keep_named_groups,
        fontdb: Database::new(),
    };

    Ok(Args {
        in_svg: in_svg.clone(),
        out_image,
        query_all: args.query_all,
        export_id,
        dump,
        perf: args.perf,
        quiet: args.perf,
        usvg,
        fit_to,
        background: args.background,
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

    resources_dir: Option<path::PathBuf>,

    query_all: bool,
    export_id: Option<String>,

    perf: bool,
    quiet: bool,
    dump_svg: Option<String>,

    input: Option<path::PathBuf>,
    output: Option<path::PathBuf>,
}

fn collect_args() -> Result<CliArgs, pico_args::Error> {
    let mut input = pico_args::Arguments::from_env();

    Ok(CliArgs {
        help: input.contains(["-h", "--help"]),
        version: input.contains(["-v", "--version"]),
        width: input.opt_value_from_fn(["-w", "--width"], parse_length)?,
        height: input.opt_value_from_fn(["-h", "--help"], parse_length)?,
        zoom: input.opt_value_from_fn(["-z", "--zoom"], parse_zoom)?,
        dpi: input.opt_value_from_fn("--dpi", parse_dpi)?.unwrap_or(96),
        background: input.opt_value_from_str("--background")?,
        resources_dir: input.opt_value_from_str("--resources-dir").unwrap_or_default(),
        query_all: input.contains("--query-all"),
        export_id: input.opt_value_from_str("--export-id")?,
        perf: input.contains("--perf"),
        quiet: input.contains("--quiet"),
        dump_svg: input.opt_value_from_str("--dump-svg")?,

        input: input.opt_value_from_fn(["-i", "--input"], parse_path)?,
        output: input.opt_value_from_fn(["-o", "--output"], parse_path)?,
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

    --query-all                 Queries all valid SVG ids with bounding boxes
    --exported-id ID            Renders an object only with a specified ID

    --perf                      Print performance stats
    --quiet                     Disables warnings
    --dump-svg PATH             Saves the preprocessed SVG into the selected file

ARGS:
    <in-svg>                    Input file
    <out-image>                 Output file
";

fn parse_length(s: &str) -> Result<u32, String> {
    let n: u32 = s.parse().map_err(|_| "Invalid length")?;

    return if n > 0 {
        Ok(n)
    } else {
        Err(String::from("Length cannot be zero"))
    };
}

fn parse_zoom(s: &str) -> Result<f32, String> {
    let n: f32 = s.parse().map_err(|_| "Invalid zoom factor")?;

    return if n > 0.0 {
        Ok(n)
    } else {
        Err(String::from("Zoom should be positive"))
    };
}

fn parse_dpi(s: &str) -> Result<u32, String> {
    let n: u32 = s.parse().map_err(|_| "Invalid number")?;
    return if n >= 10 && n <= 4000 {
        Ok(n)
    } else {
        Err(String::from("Dpi out of bounds"))
    };
}

fn parse_path(s: &str) -> Result<path::PathBuf, String> {
    return if s.is_empty() {
        Err(String::from("Path is empty"))
    } else {
        Ok(PathBuf::from(s))
    };
}

// A simple stderr logger.
static LOGGER: SimpleLogger = SimpleLogger;

struct SimpleLogger;

impl log::Log for SimpleLogger {
    fn enabled(&self, metadata: &Metadata) -> bool {
        metadata.level() <= log::LevelFilter::Warn
    }

    fn log(&self, record: &Record) {
        if self.enabled(record.metadata()) {
            let target = if record.target().len() > 0 {
                record.target()
            } else {
                record.module_path().unwrap_or_default()
            };
            let line = record.line().unwrap_or(0);

            let level_str;
            match record.level() {
                Level::Error => { level_str = "Error"; }
                Level::Warn => { level_str = "Warning"; }
                Level::Info => { level_str = "Info"; }
                Level::Debug => { level_str = "Debug"; }
                Level::Trace => { level_str = "Trace"; }
            }
            eprintln!("{} (in {}:{}): {}", level_str, target, line, record.args());
        }
    }

    fn flush(&self) {}
}

fn query_all(tree: &usvg::Tree) -> Result<(), String> {
    let mut count = 0;
    for node in tree.root().descendants() {
        if tree.is_in_defs(&node) {
            continue;
        }

        if node.id().is_empty() {
            continue;
        }

        count += 1;

        fn round_len(v: f64) -> f64 {
            (v * 1000.0).round() / 1000.0
        }

        if let Some(bbox) = node.calculate_bbox() {
            println!(
                "{}, {},{},{},{}", node.id(),
                round_len(bbox.x()), round_len(bbox.y()),
                round_len(bbox.width()), round_len(bbox.height())
            );
        }
    }

    if count == 0 {
        return Err(String::from("The file has no valid ID's"));
    }

    Ok(())
}

fn dump_svg(tree: &usvg::Tree, path: &path::Path) -> Result<(), String> {
    let mut f = std::fs::File::create(path)
        .map_err(|_| format!("Failed to create a file {:?}", path))?;

    f.write_all(tree.to_string(usvg::XmlOptions::default()).as_bytes())
        .map_err(|_| format!("Failed to write a file {:?}", path))?;

    Ok(())
}

fn render_svg(args: Args, tree: &usvg::Tree, out_image: &path::Path) -> Result<(), String> {
    let now = std::time::Instant::now();
    let img = if let Some(ref id) = args.export_id {
        if let Some(node) = tree.root().descendants()
            .find(|n| &*n.id() == id) {
            let bbox = node.calculate_bbox()
                .ok_or_else(|| String::from("Node has zero size"))?;
            let size = args.fit_to.fit_to(bbox.to_screen_size())
                .ok_or_else(|| String::from("Target size is zero"))?;

            // Unwrap is safe, because size is already valid.
            let mut pixmap = tiny_skia::Pixmap::new(size.width(), size.height()).unwrap();

            if let Some(background) = args.background {
                pixmap.fill(tiny_skia::Color::from_rgba8(
                    background.red, background.green, background.blue, 255));
            }
            svg::render_node(&node, args.fit_to, pixmap.as_mut());
            pixmap
        } else {
            return Err(format!("SVG doesn't have {} ID", id));
        }
    } else {
        let size = args.fit_to.fit_to(tree.svg_node().size.to_screen_size())
            .ok_or_else(|| String::from("Target size is zero"))?;
        println!("Svg size {}x{}", size.width(), size.height());

        // Unwrap is safe, because size is already valid.
        let mut pixmap = tiny_skia::Pixmap::new(size.width(), size.height()).unwrap();

        if let Some(background) = args.background {
            pixmap.fill(tiny_skia::Color::from_rgba8(
                background.red, background.green, background.blue, 255));
        }

        svg::render(tree, args.fit_to, pixmap.as_mut());
        pixmap
    };

    if args.perf {
        println!("Rendering: {:.2}ms", now.elapsed().as_micros() as f64 / 1000.0);
    }

    timed!(args, "Saving", img.save_png(out_image).map_err(|e| e.to_string()))
}
