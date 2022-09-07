#include "my_gl.hpp"
#include "math.hpp"
#include <algorithm>
#include <cmath>

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

void triangle(vec3f* verts, TGAImage& img, const TGAColor& color) {
    vec3f verts_i[3] {};

    for (int i = 0; i < 3; i++) {
        verts_i[i] = (vec3i)verts[i];
    }

    // DEBUGADO
    // for (int i = 0; i < 3; i++) {
    //     img.set(verts_i[i].x, verts_i[i].y, color::red);
    // }
    // line(discard_Z(verts_i[0]), discard_Z(verts_i[1]), img, color::red);
    // line(discard_Z(verts_i[2]), discard_Z(verts_i[1]), img, color::red);
    // line(discard_Z(verts_i[0]), discard_Z(verts_i[2]), img, color::red);
    // DEBUGADO

    vec2f boxmin = vec2f { img.get_width(), img.get_height() };
    vec2f boxmax {};
    for (int i = 0; i < 3; i++) {
        boxmin.x = std::max(0.0f, std::min(boxmin.x, verts_i[i].x));
        boxmin.y = std::max(0.0f, std::min(boxmin.y, verts_i[i].y));

        boxmax.x = std::min((float)img.get_width(), std::max(boxmax.x, verts_i[i].x));
        boxmax.y = std::min((float)img.get_height(), std::max(boxmax.y, verts_i[i].y));
    }

    for (int x = boxmin.x; x < boxmax.x; x++) {
        for (int y = boxmin.y; y < boxmax.y; y++) {
            vec3f bcoord = toBarycentricCoord(verts_i, vec2f { (float)x, (float)y });
            if (bcoord.x < 0.0f || bcoord.y < 0.0f || bcoord.z < 0.0f)
                continue;
            img.set(x, y, color);
        }
    }

    /// DEBUGADO
    // std::cout << "Vertices:\n";
    // for (int i = 0; i < 3; i++) {
    //     std::cout << verts[i] << "\t\t" << verts_i[i] << '\n';
    // }

    // std::cout << "Punto minimo de la caja" << boxmin << '\n';
    // std::cout << "Punto maximo de la caja" << boxmax << '\n';
    /// DEBUGADO
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

void simpleRender(Model& model, TGAImage& img) {
    vec3f light { 0, 0, 1 };
    for (int i = 0; i < model.getTotalFaces(); i++) {
        vec3f vertex1 = model.getVertex(i, 1);
        vec3f vertex2 = model.getVertex(i, 2);
        vec3f vertex3 = model.getVertex(i, 3);

        vec3f normal = crossProduct(vertex2 - vertex1, vertex3 - vertex1);
        normal.normalize();

        vertex1.x = (vertex1.x + 1.0f) * img.get_width() / 2;
        vertex1.y = (vertex1.y + 1.0f) * img.get_height() / 2;
        vertex2.x = (vertex2.x + 1.0f) * img.get_width() / 2;
        vertex2.y = (vertex2.y + 1.0f) * img.get_height() / 2;
        vertex3.x = (vertex3.x + 1.0f) * img.get_width() / 2;
        vertex3.y = (vertex3.y + 1.0f) * img.get_height() / 2;

        vec3f verts[] { vertex1, vertex2, vertex3 };
        float intensity = dotProduct(light, normal);
        if (intensity > 0.0f)
            my_gl::triangle(verts, img, TGAColor { intensity * 255, intensity * 255, intensity * 255, 255 });
    }
}

} // namespace my_gl