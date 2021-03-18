use std::collections::HashMap;
use ffmpeg_next::{format, codec, filter, ffi};
use ffmpeg_next::format::Pixel;
use ffmpeg_next::Dictionary;
use ffmpeg_next::filter::Graph;

const FILTER_DESC: &'static str = "split[x1][x2];[x1]palettegen[p];[x2][p]paletteuse";

pub struct EncodeContext {
    width: u32,
    height: u32,
    output_path: String,
    fps: u32,
    rgba_data: HashMap<u32, Vec<u8>>,
}

impl EncodeContext {
    pub fn new(width: u32, height: u32, fps: u32, path: String) -> Self {
        Self {
            width,
            height,
            output_path: path,
            fps,
            rgba_data: Default::default(),
        }
    }

    pub fn init_encode(&mut self) -> Result<(), ffmpeg_next::Error> {
        let mut output = format::output(&self.output_path)?;
        let mut octx = output.add_stream(codec::encoder::find(codec::Id::GIF))?;

        let mut encoder = octx.codec().encoder().video()?;

        encoder.set_width(self.width);
        encoder.set_height(self.height);
        encoder.set_format(Pixel::PAL8);
        encoder.set_frame_rate(Some((self.fps as i32, 1i32)));
        encoder.set_time_base((1i32, self.fps as i32));

        let encode_opt = Dictionary::new();
        encoder.open_with(encode_opt)?;
        encoder = octx.codec().encoder().video()?;

        octx.set_parameters(&encoder);

        output.write_header()?;

        filter::register_all();

        let buffer_src = filter::find("buffer").unwrap();
        let buffer_sink = filter::find("buffersink").unwrap();

        // ffi::AV_PIX_FMT_BGR32 = AV_PIX_FMT_RGBA
        let args = format!("video_size={}x{}:pix_fmt={}:time_base={}/{}:pixel_aspect={}/{}",
                           self.width, self.height, ffi::AV_PIX_FMT_BGR32 as i32, 1i32, self.fps, 1, 1);

        let mut graph = Graph::new();
        graph.add(&buffer_src, "in", &args)?;
        graph.add(&buffer_sink, "out", "")?;

        {
            let mut buffer_sink_ctx = graph.get("out").unwrap();
            buffer_sink_ctx.set_pixel_format(Pixel::PAL8);
        }

        let inputs = graph.input("out", 0)?;
        let outputs = inputs.output("in", 0)?;
        outputs.parse(&FILTER_DESC);

        graph.validate()?;

        Ok(())
    }

    pub fn add_rgba_frame(&mut self, data: Vec<u8>, index: u32) -> Result<(), ffmpeg_next::Error> {
        self.rgba_data.insert(index, data);
        Ok(())
    }
}
