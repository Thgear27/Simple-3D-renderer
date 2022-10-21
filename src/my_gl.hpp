#ifndef MY_GL_H
#define MY_GL_H

#include "math.hpp"
#include "model.hpp"
#include "tgaimage.h"
#include "matrix.hpp"

namespace color {

const TGAColor red          { 255, 0, 0, 255 };
const TGAColor white        { 255, 255, 255, 255 };
const TGAColor orange_coral { 255,127, 80, 255 };

} // namespace color

namespace my_gl {
//////////////////////////////////////////////////////////////
struct shader_i {
    virtual ~shader_i() {}
    virtual Matrix vertex(int i_face, int which_vertex) = 0;
    virtual bool fragment(const vec3f& bary_coords, TGAColor& color) = 0;
};
//////////////////////////////////////////////////////////////

void line(vec3i p0, vec3i p1, TGAImage& img, const TGAColor& color);

void triangle(Matrix* verts, float* zbuffer, TGAImage& outputImg, shader_i& shader);

void wireRender(Model& model, const TGAColor& line_color, TGAImage& img);
TGAColor getColorFromTexture (vec2f* uvCoords, vec3f baryCoords, TGAImage& textureImg);

Matrix lookAt(const vec3f& from, const vec3f& to, const vec3f& up);
Matrix viewport(int x, int y, int w, int h, int depth);

Matrix zoom(float factor);
Matrix translate(int x, int y, int z);
Matrix rotatex(float angle);
Matrix rotatey(float angle);
Matrix rotatez(float angle);
Matrix shear(float x, float y);
Matrix simpleProjection(int zDistance);

} // namespace my_gl

#endif // MY_GL_H