#include "my_gl.hpp"
#include "math.hpp"
#include <algorithm>
#include <cmath>
#include <limits>

namespace my_gl {

void line(vec3i p0, vec3i p1, TGAImage& img, const TGAColor& color) {
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

void triangle(Matrix* verts_mat, float* zbuffer, TGAImage& outputImg, shader_i& shader) {
    vec3f verts[3];
    for (int i = 0; i < 3; i++) { verts[i] = matToVec3(verts_mat[i]); }
    
    if (verts[0].y == verts[1].y && verts[0].y == verts[2].y) return;
    if (verts[0].x == verts[1].x && verts[0].x == verts[2].x) return;

    vec3f verts_i[3] {};
    float img_width = outputImg.get_width();
    float img_height = outputImg.get_height();

    for (int i = 0; i < 3; i++) {
        verts_i[i] = (vec3i)verts[i];
    }

    vec2f boxmin = vec2f { img_width, img_height };
    vec2f boxmax {};
    for (int i = 0; i < 3; i++) {
        boxmin.x = std::max(0.0f, std::min(boxmin.x, verts[i].x));
        boxmin.y = std::max(0.0f, std::min(boxmin.y, verts[i].y));

        boxmax.x = std::min(img_width, std::max(boxmax.x, verts[i].x));
        boxmax.y = std::min(img_height, std::max(boxmax.y, verts[i].y));
    }
    
    float vertex_z_value = 0;
    for (int x = boxmin.x; x < boxmax.x; x++) {
        for (int y = boxmin.y; y < boxmax.y; y++) {
            vec3f bcoord = toBarycentricCoord(verts_i, vec2f { (float)x, (float)y });

            vec3f bc_clip = vec3f { 
                bcoord.x / verts_mat[0][3][0], 
                bcoord.y / verts_mat[1][3][0], 
                bcoord.z / verts_mat[2][3][0]
            };
            bc_clip = bc_clip / (bc_clip.x + bc_clip.y + bc_clip.z) ;

            if (bcoord.x < 0.0f || bcoord.y < 0.0f || bcoord.z < 0.0f) continue;
            TGAColor color {}; 

            vertex_z_value = verts_i[0].z * bcoord.x + verts_i[1].z * bcoord.y + verts_i[2].z * bcoord.z;
            if (vertex_z_value > zbuffer[x + y * outputImg.get_width()]) {
                zbuffer[x + y * outputImg.get_width()] = vertex_z_value;
                if (!shader.fragment(bc_clip, color))
                    outputImg.set(x, y, color);
            }
        }
    }
}

void wireRender(Model& model, const TGAColor& line_color, TGAImage& img) {
    int width  = img.get_width();
    int height = img.get_height();

    vec3f* screen_coords;
    for (int i = 0; i < model.getTotalFaces(); i++) {
        screen_coords = model.getVertex_ptr(i);

        for (int vertexIndex = 0; vertexIndex < 3; vertexIndex++) {
            screen_coords[vertexIndex].x = (screen_coords[vertexIndex].x + 1.0f)  * width  / 2;
            screen_coords[vertexIndex].y = (screen_coords[vertexIndex].y + 1.0f)  * height / 2;
            screen_coords[vertexIndex].z = (screen_coords[vertexIndex].z + 1.0f)  * height / 2;
        }

        my_gl::line((vec3i)screen_coords[0], (vec3i)screen_coords[1], img, line_color);
        my_gl::line((vec3i)screen_coords[1], (vec3i)screen_coords[2], img, line_color);
        my_gl::line((vec3i)screen_coords[0], (vec3i)screen_coords[2], img, line_color);
    }
}

Matrix zoom(float factor) {
    Matrix mat = Matrix::Identity(4);
    mat[0][0] = factor; 
    mat[1][1] = factor; 
    mat[2][2] = factor; 
    return mat;
}

Matrix translate(int x, int y, int z) {
    Matrix mat = Matrix::Identity(4);
    mat[0][3] = x;
    mat[1][3] = y;
    mat[2][3] = z;
    return mat;
}

Matrix rotatex(float angle) {
    Matrix mat = Matrix::Identity(4);
    mat[1][1] = std::cos(angle);
    mat[1][2] = -std::sin(angle);
    mat[2][1] = std::sin(angle);
    mat[2][2] = std::cos(angle);
    return mat;
}

Matrix rotatey(float angle) {
    Matrix mat = Matrix::Identity(4);
    mat[0][0] = std::cos(angle);
    mat[0][2] = std::sin(angle);
    mat[2][0] = -std::sin(angle);
    mat[2][2] = std::cos(angle);
    return mat;
}

Matrix rotatez(float angle) {
    Matrix mat = Matrix::Identity(4);
    mat[0][0] = std::cos(angle);
    mat[0][1] = -std::sin(angle);
    mat[1][0] = std::sin(angle);
    mat[1][1] = std::cos(angle);
    return mat;
}


Matrix shear(float x, float y) {
    Matrix mat = Matrix::Identity(4);
    mat[0][1] = x;
    mat[1][0] = y;
    return mat;
}

Matrix simpleProjection(int zDistance) {
    Matrix mat = Matrix::Identity(4);
    mat[3][2] = -1/(float)zDistance;
    return mat;
}

Matrix lookAt(const vec3f& from, const vec3f& to, const vec3f& up) {
    vec3f foward = (from - to);      foward.normalize();
    vec3f right = crossProduct(up, foward); right.normalize();
    vec3f newup = crossProduct(foward, right);  newup.normalize();
    Matrix minv = Matrix::Identity(4);
    Matrix traslation = Matrix::Identity(4);
    minv[0] = { right.x,  right.y,  right.z,  0 };
    minv[1] = { newup.x,  newup.y,  newup.z,  0 };
    minv[2] = { foward.x, foward.y, foward.z, 0 };
    traslation[0][3] = -from.x;
    traslation[1][3] = -from.y;
    traslation[2][3] = -from.z;
    return minv * traslation;
}

Matrix viewport(int x, int y, int w, int h, int depth) {
    Matrix viewport = Matrix::Identity(4);
    viewport[0][3] = x + w / 2.0f;
    viewport[1][3] = y + h / 2.0f;
    viewport[2][3] = depth / 2.0f;

    viewport[0][0] = w / 2.0f;
    viewport[1][1] = h / 2.0f;
    viewport[2][2] = depth / 2.0f;
    return viewport;
}

} // namespace my_gl