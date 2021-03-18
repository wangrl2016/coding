use chrono::Utc;
use crate::context::{SvgContext, RenderContext};
use crate::encode::EncodeContext;

pub fn generate(svg_context: &mut SvgContext, render_context: &mut RenderContext)
                -> Result<(), Box<dyn std::error::Error>> {
    let _current = std::time::SystemTime::now();
    println!("{:?}", Utc::now());

    let gif_count = svg_context.map_gif_hrefs();
    println!("Svg中含有{}个gif文件", gif_count);

    match render_context.format {
        "gif" => {
            if gif_count > 0 {
                render_context.decode_svg_gif(svg_context.gif_hrefs.clone());
                let encode_context = EncodeContext::new(
                    svg_context.get_height(),
                    svg_context.get_height(),
                    render_context.get_fps(),
                    render_context.get_output_path());

                render_context.render(encode_context);
            }
        }

        _ => {
            println!("未支持的渲染类型: {}", render_context.format);
        }
    }

    return Ok(());
}
