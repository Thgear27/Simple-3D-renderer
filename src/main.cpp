#include "model.hpp"
#include "my_gl.hpp"
#include "tgaimage.h"
#include <fstream>
#include <iostream>

constexpr int height { 1000 };
constexpr int width { 1000 };

const char* african_head = "obj/african_head/african_head.obj";
const char* african_head_texture_img = "obj/african_head/african_head_diffuse.tga";

const char* boogie_head = "obj/boggie/head.obj";
const char* boogie_body = "obj/boggie/body.obj";
const char* diablo3pose = "obj/diablo3_pose/diablo3_pose.obj";
const char* diablo3pose_texture_img = "obj/diablo3_pose/diablo3_pose_diffuse.tga";

int main() {
    TGAImage img(width, height, TGAImage::Format::RGB);
    TGAImage textureImg;
    textureImg.read_tga_file(diablo3pose_texture_img);
    textureImg.flip_vertically();

    Model model1 { diablo3pose };

    my_gl::simpleRender(model1, textureImg, img, vec3f { 1, 0, 1 });

    img.flip_vertically();
    img.write_tga_file("model13.tga");
    std::cout << "Hecho" << '\n';
}

// TODO:
/**
 * Refactorizar el código
 * hacer que el zbuffer se comparta para dos modelos que conforman uno solo
 */
