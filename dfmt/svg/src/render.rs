use usvg::ScreenSize;
use tiny_skia::PixmapMut;

pub(crate) mod prelude {
    pub(crate) use crate::layers::Layers;
}

use prelude::*;

pub(crate) enum RenderState {
    // A default value, doesn't indicate anything.
    Ok,
    // Indicates that the current rendering task should stop after reaching the specified node.
    RenderUntil(usvg::Node),
    // Indicates the usvg::FilterInput::BackgroundImage rendering task was finished.
    BackgroundFinished,
}

pub struct Canvas<'a> {
    pub pixmap: tiny_skia::PixmapMut<'a>,
    pub transform: tiny_skia::Transform,
    pub clip: Option<tiny_skia::ClipMask>,
}

impl<'a> From<tiny_skia::PixmapMut<'a>> for Canvas<'a> {
    fn from(pixmap: PixmapMut<'a>) -> Self {
        Canvas {
            pixmap,
            transform: tiny_skia::Transform::identity(),
            clip: None,
        }
    }
}

pub(crate) fn render_to_canvas(tree: &usvg::Tree, img_size: ScreenSize,
                               canvas: &mut Canvas) {
    println!("Svg node {:?}", tree.svg_node().view_box.rect);
    render_node_to_canvas(&tree.root(), tree.svg_node().view_box,
                          img_size, &mut RenderState::Ok, canvas);
}

pub(crate) fn render_node_to_canvas(
    node: &usvg::Node, view_box: usvg::ViewBox, img_size: ScreenSize,
    state: &mut RenderState, canvas: &mut Canvas) {
    let mut layers = Layers::new(img_size);
}
