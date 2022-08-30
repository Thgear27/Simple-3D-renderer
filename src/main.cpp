#include "math.hpp"
#include "model.hpp"
#include "my_gl.hpp"
#include <fstream>
#include <iostream>

constexpr int height { 1000 };
constexpr int width { 1000 };

const char* african_head = "obj/african_head/african_head.obj";
const char* boogie_head = "obj/boggie/head.obj";
const char* boogie_body = "obj/boggie/body.obj";
const char* diablo3pose = "obj/diablo3_pose/diablo3_pose.obj";

int main() {
    TGAImage img(height, width, TGAImage::Format::RGB);
    Model modelo;
    Model modelo2;

    if (!modelo.load_model_from_file(boogie_body))
        return -1;

    if (!modelo2.load_model_from_file(boogie_head))
        return -1;

    my_gl::wireRender(modelo, color::red, img);

    img.flip_vertically();
    img.write_tga_file("renders/file6.tga");
}

// TODO:
/**
 * Refactorizar el código  
 * Dibujar (rellenarlas) las faces sin preocuparse del zbuffer
 */