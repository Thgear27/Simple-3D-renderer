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
    Model model1 { boogie_body };
    vec3f light { 0, 0, 1 };

    my_gl::simpleRender(model1, img);

    img.flip_vertically();
    img.write_tga_file("test7.tga");
    std::cout << "hecho" << '\n';
}

// TODO:
/**
 * Refactorizar el código
 */