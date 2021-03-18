use chrono::Utc;
use crate::context::{SvgContext, RenderContext};

pub fn render(svg_context: &mut SvgContext, render_context: &RenderContext)
              -> Result<(), Box<dyn std::error::Error>> {
    let _current = std::time::SystemTime::now();
    println!("{:?}", Utc::now());

    let gif_count = svg_context.map_gif_hrefs();
    println!("Svg中含有{}个gif文件", gif_count);

    match render_context.format {
        "gif" => {}

        _ => {
            println!("未支持的渲染类型: {}", render_context.format);
            if gif_count > 0 {


            }
        }
    }

    return Ok(());
}
