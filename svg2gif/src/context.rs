// 保留和svg相关的信息
pub struct SvgContext<'a> {
    // 以像素为单位
    width: u32,
    // 以像素为单位
    height: u32,
    doc: roxmltree::Document<'a>,
    tree: Option<usvg::Tree>,
}

impl<'a> SvgContext<'a> {
    pub fn new(source: &'a str) -> Option<SvgContext<'a>> {
        let document = roxmltree::Document::parse(source).unwrap();
        let width = document.root_element().attribute("width")
            .and_then(|c| Some(c.parse::<f64>().unwrap_or(0.0)))
            .unwrap_or(0.0).round() as u32;
        let height = document.root_element().attribute("height")
            .and_then(|c| Some(c.parse::<f64>().unwrap_or(0.0)))
            .unwrap_or(0.0).round() as u32;

        if width == 0 || height == 0 {
            println!("不兼容没有宽和高属性的svg");
            ()
        } else {
            println!("Svg size {}x{}", width, height);
        }

        Some(Self {
            width,
            height,
            doc: document,
            tree: None,
        })
    }
}

// 保留和渲染相关的信息
pub struct RenderContext<'a> {
    // 输出路径
    output: &'a str,
    // 输出格式
    format: &'a str,
    // 输出帧率
    fps: u32,
    // 输出大小
}

impl<'a> RenderContext<'a> {
    pub fn new(output: &'a str, format: &'a str, fps: u32) -> Option<RenderContext<'a>> {
        Some(Self {
            output,
            format,
            fps,
        })
    }
}
