use std::collections::HashMap;
use ffmpeg_next::format::{input, Pixel};
use ffmpeg_next::util::media::Type;
use ffmpeg_next::software::scaling::{context, flag};
use ffmpeg_next::frame::Video;
use crate::utils::save_ppm_file;
use ffmpeg_next::sys::AV_PIX_FMT_BGR32;

// 存放解码后的gif帧
pub struct FrameContainer {
    // 分离后的所有gif字符串
    splice: Vec<String>,
    // 字符串对应的下标
    // 以及(width, height, data)存放的解码数据
    // 默认保存为rgba格式
    cache: HashMap<usize, Vec<(u32, u32, Vec<u8>)>>,
}

impl FrameContainer {
    pub fn new() -> Self {
        FrameContainer {
            splice: vec![],
            cache: Default::default(),
        }
    }

    pub fn decode_gif(&mut self, path: &str, index: u32) -> Result<(), ffmpeg_next::Error> {
        ffmpeg_next::init().expect("FFmpeg init failed");

        if let Ok(mut ictx) = input(&path) {
            let input = ictx
                .streams()
                .best(Type::Video)
                .ok_or_else(|| ffmpeg_next::Error::StreamNotFound)?;

            let video_stream_index = input.index();

            let mut decoder = input.codec().decoder().video()?;

            let mut scaler = context::Context::get(
                decoder.format(),
                decoder.width(),
                decoder.height(),
                Pixel::RGB24,
                decoder.width(),
                decoder.height(),
                flag::Flags::BILINEAR)?;

            let mut frame_index = 0;

            let mut threads = Vec::new();

            let mut receive_and_process_decoded_frames =
                |decoder: &mut ffmpeg_next::decoder::Video| -> Result<(), ffmpeg_next::Error> {
                    let mut decoded = Video::empty();
                    while decoder.receive_frame(&mut decoded).is_ok() {
                        let mut rgba_frame = Video::empty();
                        let line_size = unsafe { (*decoded.as_mut_ptr()).linesize[0] } as u32;
                        if line_size != decoded.width() * 4 {
                            rgba_frame.set_width(decoded.width());
                            rgba_frame.set_height(decoded.height());
                            rgba_frame.set_format(Pixel::RGB24);
                            unsafe { ffmpeg_next::ffi::av_frame_get_buffer(rgba_frame.as_mut_ptr(), 1); }
                        }

                        scaler.run(&decoded, &mut rgba_frame)?;

                        let mut timestamp;
                        match decoded.timestamp() {
                            Some(t) => { timestamp = t; }
                            _ => { panic!("No timestamp"); }
                        }

                        threads.push(std::thread::spawn(move || {
                            let data = rgba_frame.data(0).to_vec();
                            println!("data size {}", data.len());
                            let width = rgba_frame.width();
                            let height = rgba_frame.height();

                            // 保存为ppm图片格式
                            println!("{}", timestamp);
                            if timestamp < 40 {
                                save_ppm_file(&*format!("out/{}.ppm", timestamp),
                                              Pixel::RGB24, width, height, data);
                            }
                        }));
                    }

                    Ok(())
                };

            for (stream, packet) in ictx.packets() {
                if stream.index() == video_stream_index {
                    decoder.send_packet(&packet)?;
                    receive_and_process_decoded_frames(&mut decoder);
                }
            }

            decoder.send_eof()?;
            receive_and_process_decoded_frames(&mut decoder)?;
            for thread in threads {
                thread.join().unwrap();
            }
        }
        Ok(())
    }
}
