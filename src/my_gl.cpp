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

TGAColor getColorFromTexture(vec2f* uvCoords, vec3f baryCoords, TGAImage& textureImg) {
    // vec2i uvCoords_int[3];
    int img_width = textureImg.get_width();
    int img_height = textureImg.get_height();

    vec2f uvCoordsCpy[3];
    for (int i = 0; i < 3; i++) {
        uvCoordsCpy[i] = uvCoords[i];
    }

    for (int i = 0; i < 3; i++) {
        uvCoordsCpy[i].x = uvCoords[i].x * img_width;
        uvCoordsCpy[i].y = uvCoords[i].y * img_height;
    }
    vec2f point = uvCoordsCpy[0] * baryCoords.x + uvCoordsCpy[1] * baryCoords.y + uvCoordsCpy[2] * baryCoords.z;
    return textureImg.get((int)point.x, (int)point.y);
}

float getSmoothIntensity(vec3f* vec_normals, vec3f bcoord, vec3f lightDirection) {
    vec3f normalResult = vec_normals[0] * bcoord.x + vec_normals[1] * bcoord.y + vec_normals[2] * bcoord.z ;
    float intensity = dotProduct(normalResult, lightDirection);
    return (intensity < 0.0f) ?  0.0f : intensity;
}

float getFaceIntensity(vec3f* modelVerts, vec3f lightDirNormalized) {
    vec3f normal = crossProduct(modelVerts[1] - modelVerts[0], modelVerts[2] - modelVerts[0]);
    normal.normalize();
    float intensity = dotProduct(normal, lightDirNormalized);
    return (intensity < 0.0f) ?  0.0f : intensity;
}

void triangle(vec3f* verts, float* zbuffer, TGAImage& textureImg, vec2f* uvCoords, TGAImage& outputImg, vec3f* vec_normals, vec3f lightDir, bool smoothShadow) {
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
            if (bcoord.x < 0.0f || bcoord.y < 0.0f || bcoord.z < 0.0f)
                continue;

            TGAColor TxturePxlColor = getColorFromTexture(uvCoords, bcoord, textureImg);
            
            if (smoothShadow) {
                float intensity = getSmoothIntensity(vec_normals, bcoord, lightDir);
                for (int i = 0; i < 3; i++) {
                    TxturePxlColor.raw[i] = intensity * TxturePxlColor.raw[i];
                }
            } else {
                float intensity = getFaceIntensity(verts, lightDir);
                for (int i = 0; i < 3; i++) {
                    TxturePxlColor.raw[i] = intensity * TxturePxlColor.raw[i];
                }
            }

            vertex_z_value = verts_i[0].z * bcoord.x + verts_i[1].z * bcoord.y + verts_i[2].z * bcoord.z;
            if (vertex_z_value > zbuffer[x + y * outputImg.get_width()]) {
                zbuffer[x + y * outputImg.get_width()] = vertex_z_value;
                outputImg.set(x, y, TxturePxlColor);
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

void simpleRender(Model& model, float* img_zbuffer ,TGAImage& outputImg, vec3f lightDir, Matrix vpm) {
    int width  = outputImg.get_width();
    int height = outputImg.get_height();

    lightDir.normalize();
    vec2f mvc[3] = { vec2f { 0.0f, 0.0f }, vec2f { 0.9f, 0.9f }, vec2f { 0.0f, 0.9f }};
    vec2f* modelVtCoords; 
    vec3f* screen_coords = new vec3f[3] {};
    vec3f* modelVecNormals;
    if (model.getFormat() == Model::Format::no_vt) modelVtCoords = mvc;
    for (int i = 0; i < model.getTotalFaces(); i++) {
        if (model.getFormat() == Model::Format::with_vt)
            modelVtCoords = model.getVertexTexture_ptr(i);

        modelVecNormals = model.getVertexNormal_ptr(i);

        for (int vi = 0; vi < 3; vi++) {
            screen_coords[vi] = matToVec3 (
                  vpm
                * vecToMat(model.getVertex_ptr(i)[vi])
            );
            std::cout << screen_coords[vi] << '\n';
        }

        my_gl::triangle(screen_coords, img_zbuffer, model.getTextureImg(), modelVtCoords, outputImg, modelVecNormals, lightDir, true);
    }
    delete[] screen_coords;
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

} // namespace my_gl