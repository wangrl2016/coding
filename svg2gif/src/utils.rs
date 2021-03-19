use std::fs::File;
use std::io::Write;
use ffmpeg_next::Format;
use ffmpeg_next::format::Pixel;
use ffmpeg_next::sys::AV_PIX_FMT_BGR32;
use chrono::{Local, Timelike};

// 创建嵌套目录
fn create_file_dir(path: &str) {
    let split_line_index = path.rfind('/').unwrap_or_else(|| 0);
    if split_line_index > 0 {
        if !std::path::Path::new(&path).exists() {
            std::fs::create_dir_all(&path[0..split_line_index]).unwrap();
        }
    }
}

//　整体当前时间的打印格式
pub fn get_current_time() -> String {
    let now = Local::now();
    return format!("{:02}:{:02}:{:02}.{:03}",
                   now.hour(), now.minute(), now.second(), now.timestamp_subsec_millis());
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

    pub fn delete(self) {
        std::fs::remove_file(self.0).unwrap();
    }
}

pub fn save_ppm_file(path: &str, pixel_format: Pixel, width: u32, height: u32, data: Vec<u8>) {
    create_file_dir(path);
    let mut file = File::create(path).unwrap();
    // let mut pixel_bits;
    // if pixel_format == Pixel::RGBA {
    //     pixel_bits = 4;
    // } else {
    //     println!("Do not save to ppm file");
    //     return;
    // }
    file.write("P6".as_ref());
    file.write("\n".as_ref());
    file.write(width.to_string().as_ref());
    file.write(" ".as_ref());
    file.write(height.to_string().as_ref());
    file.write("\n".as_ref());
    file.write("255\n".as_ref());

    println!("size {}x{} {}, ", width, height, width * height * 4);

    file.write(data.as_ref());
    // for j in 0..height {
    //     for i in 0..width {
    //         let mut pixel = vec![];
    //         pixel.push(&data[j * width * 4 + i * 4]);
    //         pixel.push(&data[j * width * 4 + i * 4 + 1]);
    //         pixel.push(&data[j * width + 4 + i * 4 + 2]);
    //         file.write(pixel.as_ref());
    //     }
    // }
    file.try_clone();
}
