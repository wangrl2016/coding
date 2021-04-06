mod decoder;
mod parser;
mod marker;

use std::fs::File;
use std::io::BufReader;


fn main() {
    let file = File::open("../../res/yuvj420p-1600x1067.jpeg").expect("Failed to open file");
    let mut decoder = jpeg_decoder::Decoder::new(BufReader::new(file));
    let _pixels = decoder.decode().expect("Failed to decode image");
    let metadata = decoder.info().unwrap();
    println!("{:?}", metadata);

    let f = File::open("../../res/yuvj420p-1600x1067.jpeg").expect("Failed to open file");
    let mut self_decoder = crate::decoder::Decoder::new(f);
    self_decoder.decode().expect("Failed to decode image");
    let meta = self_decoder.info().unwrap();
    println!("{:?}", meta);
}
