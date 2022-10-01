#ifndef MY_GL_H
#define MY_GL_H

#include "math.hpp"
#include "model.hpp"
#include "tgaimage.h"

namespace color {

const TGAColor red     { 255, 0, 0, 255 };
const TGAColor white   { 255, 255, 255, 255 };

} // namespace color

namespace my_gl {

void line(vec3i p0, vec3i p1, TGAImage& img, const TGAColor& color);

void triangle(vec3f* verts, float* zbuffer, TGAImage& textureImg, vec2f* uvCoords, TGAImage& outputImg, float intensity);

void wireRender(Model& model, const TGAColor& line_color, TGAImage& img);
void simpleRender(Model& model,TGAImage& textureImg, float* img_zbuffer,TGAImage& outputImg, vec3f lightDirection);
TGAColor getColorFromTexture (vec2f* uvCoords, vec3f baryCoords, TGAImage& textureImg);

Matrix zoom(float factor);
Matrix translate(int x, int y, int z);
Matrix rotate(float angle);
Matrix shear(float x, float y);

} // namespace my_gl

#endif // MY_GL_H