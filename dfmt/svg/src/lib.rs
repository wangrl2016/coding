mod render;
mod layers;

// Renders an SVG to pixmap.
// If fit_to size differs from tree.svg_node.size, SVG would be scaled accordingly.
pub fn render(
    tree: &usvg::Tree,
    fit_to: usvg::FitTo,
    pixmap: tiny_skia::PixmapMut) -> Option<()> {
    let size = fit_to.fit_to(tree.svg_node().size.to_screen_size())?;
    let mut canvas = render::Canvas::from(pixmap);
    render::render_to_canvas(tree, size, &mut canvas);
    Some(())
}

pub fn render_node(
    _node: &usvg::Node,
    _fit_to: usvg::FitTo,
    _pixmap: tiny_skia::PixmapMut) -> Option<()> {
    Some(())
}