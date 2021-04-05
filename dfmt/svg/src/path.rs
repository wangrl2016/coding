use crate::render::prelude::*;
use crate::render::Canvas;

pub fn draw(tree: &usvg::Tree, path: &usvg::Path,
            blend_mode: tiny_skia::BlendMode, canvas: &mut Canvas) -> Option<Rect> {
    let bbox = path.data.bbox();
    if path.visibility != usvg::Visibility::Visible {
        return bbox;
    }

    // usvg guaranties that path without a bbox will not use
    // a paint server with ObjectBoundingBox,
    // so we can pass whatever rect we want, because it will not be used anyway.
    let style_bbox = bbox.unwrap_or_else(|| Rect::new(0.0, 0.0, 1.0, 1.0).unwrap());

    let skia_path = convert_path(&path.data)?;
    let antialias = path.rendering_mode.use_shape_antialiasing();

    bbox
}