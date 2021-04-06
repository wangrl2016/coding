use crate::parser::FrameInfo;
use std::io::Read;
use crate::marker::Marker;

fn read_u8<R: Read>(reader: &mut R) -> std::io::Result<u8> {
    let mut buf = [0];
    reader.read_exact(&mut buf)?;
    return Ok(buf[0]);
}

// An enumeration over combination of color spaces and bit depths a pixel can have.
#[derive(Clone, Copy, Debug, PartialEq)]
pub enum PixelFormat {
    // Luminance (grayscale), 8 bits
    L8,
    // RGB, 8 bits per channel.
    RGB24,
    // CMYK, 8 bits per channel.
    CMYK32,
}

#[derive(Clone, Copy, Debug, PartialEq)]
pub struct ImageInfo {
    // The width of the image, in pixels.
    pub width: u16,
    pub height: u16,
    pub pixel_format: PixelFormat,
}

pub struct Decoder<R> {
    reader: R,
    frame: Option<FrameInfo>,
}

impl<R: Read> Decoder<R> {
    pub fn new(reader: R) -> Decoder<R> {
        Decoder {
            reader,
            frame: None,
        }
    }

    pub fn info(&self) -> Option<ImageInfo> {
        Some(ImageInfo {
            width: 0,
            height: 0,
            pixel_format: PixelFormat::L8,
        })
    }
    pub fn decode(&mut self) -> Result<Vec<u8>, String> {
        self.decode_internal(false)
    }

    fn decode_internal(&mut self, stop_after_metadata: bool) -> Result<Vec<u8>, String> {
        if stop_after_metadata && self.frame.is_some() {
            // The metadata has already been read.
            return Ok(Vec::new());
        } else if self.frame.is_none() && (read_u8(&mut self.reader).unwrap() != 0xFF ||
            Marker::from_u8(read_u8(&mut self.reader).unwrap()) != Some(Marker::SOI)) {
            // Segment SOI(Start of Image) FF D8
            return Err(String::from("First two bytes are not an SOI marker"));
        }

        let mut previous_marker = Marker::SOI;
        // let mut pending_marker = None;
        // let mut worker = None;
        let mut scans_processed = 0;

        return Ok(Vec::new());
    }
}