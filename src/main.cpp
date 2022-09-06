#include "model.hpp"
#include "my_gl.hpp"
#include "tgaimage.h"
#include <fstream>
#include <iostream>

constexpr int height { 1000 };
constexpr int width { 1000 };

const char* african_head = "obj/african_head/african_head.obj";
const char* boogie_head = "obj/boggie/head.obj";
const char* boogie_body = "obj/boggie/body.obj";
const char* diablo3pose = "obj/diablo3_pose/diablo3_pose.obj";

int main() {
    vec3f vector { 1.999f, 3.434f, 56.234 };
    vec3i vectori = vector;
    std::cout << vectori << '\n';

    TGAImage img(height, width, TGAImage::Format::RGB);
    Model modelo;
    Model modelo2;

    if (!modelo.load_model_from_file(boogie_body))
        return -1;

    if (!modelo2.load_model_from_file(boogie_head))
        return -1;

    my_gl::wireRender(modelo, color::red, img);
    my_gl::wireRender(modelo2, color::red, img);

    img.flip_vertically();
    img.write_tga_file("renders/file6.tga");
}

// TODO:
/**
 * Refactorizar el código
 * Dibujar (rellenarlas) las faces sin preocuparse del zbuffer
 */