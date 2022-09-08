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
    TGAImage img(width, height, TGAImage::Format::RGB);
    Model model1 { diablo3pose };

    my_gl::simpleRender(model1, img);

    img.flip_vertically();
    img.write_tga_file("diablo3pose4.tga");
    std::cout << "Hecho" << '\n';
}

// TODO:
/**
 * Refactorizar el código
 * hacer que el zbuffer se comparta para dos modelos que conforman uno solo
 */