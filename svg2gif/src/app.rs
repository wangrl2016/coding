use chrono::{Utc, TimeZone};
use std::convert::TryFrom;
use crate::context::{SvgContext, RenderContext};
use usvg::Svg;
use std::fmt::Pointer;

pub fn render(svg_context: &SvgContext, render_context: &RenderContext)
              -> Result<(), Box<dyn std::error::Error>> {
    let _current = std::time::SystemTime::now();
    println!("{:?}", Utc::now());


    return Ok(());
}
