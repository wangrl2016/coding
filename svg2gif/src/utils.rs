use std::fs::File;
use std::io::Write;
use ffmpeg_next::Format;
use ffmpeg_next::format::Pixel;

// 创建嵌套目录
fn create_file_dir(path: &str) {
    let split_line_index = path.rfind('/').unwrap_or_else(|| 0);
    if split_line_index > 0 {
        if !std::path::Path::new(&path).exists() {
            std::fs::create_dir_all(&path[0..split_line_index]).unwrap();
        }
    }
}

pub struct TempFile(pub(crate) String);

// 保存数据到文件中
impl TempFile {
    pub fn save(path: &str, data: &[u8]) -> Self {
        create_file_dir(path);
        let mut file = File::create(format!("{}", &path)).unwrap();
        file.write(data).unwrap();
        TempFile(path.to_string())
    }
}

pub fn save_ppm_file(path: &str, pixel_format: Pixel, width: u32, height: u32, data: Vec<u8>) {
    let file = File::create(path).unwrap();

}
