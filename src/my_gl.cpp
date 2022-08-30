#include "my_gl.hpp"
#include "math.hpp"
#include <cmath>
#include <algorithm>

namespace my_gl {

void line(vec2i p0, vec2i p1, TGAImage& img, const TGAColor& color) {

    bool inverted_plane = false;
    if (std::abs(p1.y - p0.y) > std::abs(p1.x - p0.x)) { // Triangulo con pendiente mayor a 1
        std::swap(p0.x, p0.y);
        std::swap(p1.x, p1.y);
        inverted_plane = true;
    }

    if (p0.x > p1.x) {
        std::swap(p0, p1);
    }

    // Bresenham's algorithm
    int dx = p1.x - p0.x;
    int dy = p1.y - p0.y;
    int dxe = std::abs(dy) - std::abs(dx);
    int y = p0.y; // punto inicial que ira aumentando
    for (int x = p0.x; x <= p1.x; x++) {
        if (!inverted_plane)
            img.set(x, y, color);
        else
            img.set(y, x, color);

        dxe += std::abs(dy);

        if (dxe > 0) {
            dxe -= dx;
            y += (p1.y > p0.y) ? 1 : -1;
        }
    }
}

void wireRender(Model& model, const TGAColor& line_color, TGAImage& img) {
    for (int i = 0; i < model.getTotalFaces(); i++) {
        vec2f vertex1 = discard_Z(model.getVertex(i, 1));
        vec2f vertex2 = discard_Z(model.getVertex(i, 2));
        vec2f vertex3 = discard_Z(model.getVertex(i, 3));
        
        vertex1.x = (vertex1.x + 1.0f) * img.get_width() / 2;
        vertex1.y = (vertex1.y + 1.0f) * img.get_height() / 2;
        vertex2.x = (vertex2.x + 1.0f) * img.get_width() / 2;
        vertex2.y = (vertex2.y + 1.0f) * img.get_height() / 2;
        vertex3.x = (vertex3.x + 1.0f) * img.get_width() / 2;
        vertex3.y = (vertex3.y + 1.0f) * img.get_height() / 2;

        vec2i v1 { (int)vertex1.x, (int)vertex1.y };
        vec2i v2 { (int)vertex2.x, (int)vertex2.y };
        vec2i v3 { (int)vertex3.x, (int)vertex3.y };

        my_gl::line(v1, v2, img, line_color);
        my_gl::line(v1, v3, img, line_color);
        my_gl::line(v2, v3, img, line_color);
    }
}

} // namespace my_gl