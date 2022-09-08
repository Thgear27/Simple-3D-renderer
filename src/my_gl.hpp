#ifndef MY_GL_H
#define MY_GL_H

#include "math.hpp"
#include "model.hpp"
#include "tgaimage.h"

namespace color {

const TGAColor red { 255, 0, 0, 255 };
const TGAColor white = { 255, 255, 255, 255 };

} // namespace color

namespace my_gl {

void line(vec2i p0, vec2i p1, TGAImage& img, const TGAColor& color);

void triangle(vec3f* verts, float* zbuffer, TGAImage& img, const TGAColor& color);

void wireRender(Model& model, const TGAColor& line_color, TGAImage& img);
void simpleRender(Model& model, TGAImage& img);

} // namespace my_gl

#endif // MY_GL_H