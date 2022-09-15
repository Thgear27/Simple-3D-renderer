#include "my_gl.hpp"
#include "math.hpp"
#include <algorithm>
#include <cmath>
#include <limits>

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

TGAColor getColorFromTexture(vec2f* uvCoords, vec3f baryCoords, TGAImage& textureImg) {
    // vec2i uvCoords_int[3];
    vec2f uvCoordsCpy[3];
    for (int i = 0; i < 3; i++) {
        uvCoordsCpy[i] = uvCoords[i];
    }

    for (int i = 0; i < 3; i++) {
        uvCoordsCpy[i].x = uvCoords[i].x * textureImg.get_width();
        uvCoordsCpy[i].y = uvCoords[i].y * textureImg.get_height();
    }
    vec2f point = uvCoordsCpy[0] * baryCoords.x + uvCoordsCpy[1] * baryCoords.y + uvCoordsCpy[2] * baryCoords.z;
    return textureImg.get((int)point.x, (int)point.y);
}

void triangle(vec3f* verts, float* zbuffer, TGAImage& textureImg, vec2f* uvCoords, TGAImage& outputImg,
              float intensity) {
    vec3f verts_i[3] {};

    for (int i = 0; i < 3; i++) {
        verts_i[i] = (vec3i)verts[i];
    }

    vec2f boxmin = vec2f { (float)outputImg.get_width(), (float)outputImg.get_height() };
    vec2f boxmax {};
    for (int i = 0; i < 3; i++) {
        boxmin.x = std::max(0.0f, std::min(boxmin.x, verts_i[i].x));
        boxmin.y = std::max(0.0f, std::min(boxmin.y, verts_i[i].y));

        boxmax.x = std::min((float)outputImg.get_width(), std::max(boxmax.x, verts_i[i].x));
        boxmax.y = std::min((float)outputImg.get_height(), std::max(boxmax.y, verts_i[i].y));
    }

    float vertex_z_value = 0;
    for (int x = boxmin.x; x < boxmax.x; x++) {
        for (int y = boxmin.y; y < boxmax.y; y++) {
            vec3f bcoord = toBarycentricCoord(verts_i, vec2f { (float)x, (float)y });
            if (bcoord.x < 0.0f || bcoord.y < 0.0f || bcoord.z < 0.0f)
                continue;

            TGAColor TexturePixelColor = getColorFromTexture(uvCoords, bcoord, textureImg);
            for (int i = 0; i < 3; i++) {
                TexturePixelColor.raw[i] = (intensity < 0.0f) ? 0 : intensity * TexturePixelColor.raw[i];
            }

            vertex_z_value = verts_i[0].z * bcoord.x + verts_i[1].z * bcoord.y + verts_i[2].z * bcoord.z;
            if (vertex_z_value > zbuffer[x + y * outputImg.get_width()]) {
                zbuffer[x + y * outputImg.get_width()] = vertex_z_value;
                outputImg.set(x, y, TexturePixelColor);
            }
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

void simpleRender(Model& model, TGAImage& textureImg, TGAImage& outputImg, vec3f lightDirection) {
    int width = outputImg.get_width();
    int height = outputImg.get_height();
    float* zbuffer = new float[width * height];
    for (int i = 0; i < width * height; i++) {
        zbuffer[i] = -std::numeric_limits<float>::max();
    }

    lightDirection.normalize();

    vec2f* modelUvCoords = new vec2f[3];
    for (int i = 0; i < model.getTotalFaces(); i++) {
        for (int coordIndex = 0; coordIndex < 3; coordIndex++) {
            modelUvCoords[coordIndex] = model.getVertexTexture(i, coordIndex + 1);
        }

        vec3f vertex1 = model.getVertex(i, 1);
        vec3f vertex2 = model.getVertex(i, 2);
        vec3f vertex3 = model.getVertex(i, 3);

        vertex1.x = (vertex1.x + 1.0f) * width / 2 + 0.5f;
        vertex1.y = (vertex1.y + 1.0f) * height / 2 + 0.5f;
        vertex1.z = (vertex1.z + 1.0f) * height / 2 + 0.5f;
        // vertex1.z = (vertex1.z + 1.0f) * width;

        vertex2.x = (vertex2.x + 1.0f) * width / 2 + 0.5f;
        vertex2.y = (vertex2.y + 1.0f) * height / 2 + 0.5f;
        vertex2.z = (vertex2.z + 1.0f) * height / 2 + 0.5f;
        // vertex2.z = (vertex2.z + 1.0f) * width;

        vertex3.x = (vertex3.x + 1.0f) * width / 2 + 0.5f;
        vertex3.y = (vertex3.y + 1.0f) * height / 2 + 0.5f;
        vertex3.z = (vertex3.z + 1.0f) * height / 2 + 0.5f;
        // vertex3.z = (vertex3.z + 1.0f) * width;

        // vec3f normal = model.getVertexNormal(i, 1) + model.getVertexNormal(i, 2) + model.getVertexNormal(i, 3);
        vec3f normal = crossProduct(vertex2 - vertex1, vertex3 - vertex1);
        normal.normalize();

        vec3f verts[] { vertex1, vertex2, vertex3 };
        float intensity = dotProduct(lightDirection, normal);

        // if (intensity > 0.0f) {
        int rgb = (intensity < 0.0f) ? 0 : intensity * 255;
        my_gl::triangle(verts, zbuffer, textureImg, modelUvCoords, outputImg, intensity);
        // }
        // my_gl::triangle(verts, zbuffer, img, TGAColor { rand() % 255, rand() % 255, rand() % 255, 255 });
    }
    delete[] modelUvCoords;
    delete[] zbuffer;
}

} // namespace my_gl