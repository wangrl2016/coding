use chrono::Utc;
use std::convert::TryFrom;

const _FPS: i32 = 24;


/// 渲染图片核心函数
/// source: 输入svg的路径
/// output: 输出图片的路径
/// format: 输出图片的格式
pub fn render(source: &str, _output: &str, format: &str)
              -> Result<(), Box<dyn std::error::Error>> {
    let _current = std::time::SystemTime::now();
    println!("{:?}", Utc::now());

    let _doc = roxmltree::Document::parse(source).unwrap();

    match format {
        "gif" => {}

        "jpg" | "jpeg" | "png" => {}

        _ => {
            println!("Unsupported format: {}", format);
            return Err(Box::try_from("Unsupported format").unwrap());
        }
    }


    return Ok(());
}
