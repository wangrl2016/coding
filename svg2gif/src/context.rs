

lazy_static! {
    static ref IMAGE_INDEXES: Vec<String> = vec![
        "iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAAC0lEQVQYV2NgAAIAAAUAAarVyFEAAAAASUVORK5CYII=".to_string(),
        "iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVQYV2NgYGD8DwABBgEB8Oyl4AAAAABJRU5ErkJggg==".to_string(),
        "iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVQYV2NgYGD6DwABCAECAKvsagAAAABJRU5ErkJggg==".to_string(),
    ];
}
// 保留和svg相关的信息
pub struct SvgContext<'a> {
    // 以像素为单位
    width: u32,
    // 以像素为单位
    height: u32,
    doc: roxmltree::Document<'a>,
    tree: Option<usvg::Tree>,
    source: &'a str,
    gif_hrefs: Vec<String>,
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
            source,
            gif_hrefs: vec![],
        })
    }

    pub fn map_gif_hrefs(&mut self) -> u32 {
        let mut hrefs = Vec::<String>::new();
        let mut input = self.source.to_string();
        // svg中含有的gif个数
        let mut gif_count = 0u32;
        for ref node in self.doc.descendants() {
            if node.has_tag_name("image") {
                let href = node.attribute("href")
                    .unwrap_or_else(|| node.attribute("xlink:href")
                        .unwrap_or_else(|| node.attribute(("http://www.w3.org/1999/xlink", "href"))
                            .unwrap_or("")));
                if !href.is_empty() && href.starts_with("data:image/gif;base64") &&
                    !hrefs.contains(&href.to_string()) {
                    // 将base64形式存在的gif放置在vector中
                    hrefs.push(href.to_string());
                    let re_href: String;
                    match IMAGE_INDEXES.get(gif_count as usize) {
                        Some(s) => { re_href = s.to_string(); }
                        _ => { panic!("Max gif count is {}", IMAGE_INDEXES.len()); }
                    }
                    input = input.replace(href, &format!("data:image/png;base64,{}", re_href));
                    gif_count += 1;
                }
            }
        }
        self.gif_hrefs = hrefs;
        return gif_count;
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

    // 获取输出文件的类型
    pub fn get_format() {}
}
