use usvg::{ScreenSize, NodeExt};
use tiny_skia::{PixmapMut, Transform};

pub(crate) mod prelude {
    pub(crate) use usvg::*;
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

impl Canvas<'_> {
    pub fn apply_transform(&mut self, ts: tiny_skia::Transform) {
        self.transform = self.transform.pre_concat(ts);
    }
}

pub(crate) trait ConvTransform {
    fn to_native(&self) -> tiny_skia::Transform;
    fn from_native(_: tiny_skia::Transform) -> Self;
}

impl ConvTransform for usvg::Transform {
    fn to_native(&self) -> Transform {
        tiny_skia::Transform::from_row(
            self.a as f32, self.b as f32,
            self.c as f32, self.d as f32,
            self.e as f32, self.d as f32,
        )
    }

    fn from_native(ts: tiny_skia::Transform) -> Self {
        Self::new(
            ts.sx as f64, ts.ky as f64,
            ts.kx as f64, ts.sy as f64,
            ts.tx as f64, ts.ty as f64,
        )
    }
}

// Applies viewbox transformation to the painter.
fn apply_viewbox_transform(view_box: usvg::ViewBox, img_size: ScreenSize, canvas: &mut Canvas) {
    let ts = usvg::utils::view_box_to_transform(view_box.rect, view_box.aspect,
                                                img_size.to_size());
    canvas.apply_transform(ts.to_native());
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

    apply_viewbox_transform(view_box, img_size, canvas);

    let curr_ts = canvas.transform.clone();

    let mut ts = node.abs_transform();
    ts.append(&node.transform());

    canvas.apply_transform(ts.to_native());
    render_node(node, state, &mut layers, canvas);
    canvas.transform = curr_ts;
}

pub(crate) fn render_node(node: &usvg::Node, state: &mut RenderState,
                          layers: &mut Layers, canvas: &mut Canvas) -> Option<Rect> {
    match *node.borrow() {
        usvg::NodeKind::Path(ref path) => {
            crate::path::draw(&node.tree(), path, tiny_skia::BlendMode::SourceOver, canvas)
        }

        _ => None,
    }
}
