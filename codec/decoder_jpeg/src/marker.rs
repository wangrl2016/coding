use crate::marker::Marker::SOI;

#[derive(Clone, Copy, Debug, PartialEq)]
pub enum Marker {
    // Start of Frame markers.
    SOF(u8),
    // Reserved for JPEG extensions.
    JPG,
    // Define Huffman table(s).
    DHT,
    // Start of Image.
    SOI,
    // End of image.
    EOI,
}

impl Marker {
    pub fn from_u8(n: u8) -> Option<Marker> {
        match n {
            0xD8 => Some(SOI),
            _ => None,
        }
    }
}