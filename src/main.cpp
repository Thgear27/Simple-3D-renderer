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
const char* boogie_head_texture_img = "obj/boggie/head_diffuse.tga";

const char* boogie_body = "obj/boggie/body.obj";
const char* boogie_body_texture_img = "obj/boggie/body_diffuse.tga";

const char* diablo3pose = "obj/diablo3_pose/diablo3_pose.obj";
const char* diablo3pose_texture_img = "obj/diablo3_pose/diablo3_pose_diffuse.tga";

int main() {
    Model model1 { african_head, Model::Format::with_vt };

    TGAImage img(width, height, TGAImage::Format::RGB);
    TGAImage textureImg;

    textureImg.read_tga_file(african_head_texture_img);
    textureImg.flip_vertically();

    float* zbuffer = new float[width * height];

    for (int i = 0; i < width * height; i++) {
        zbuffer[i] = -std::numeric_limits<float>::max();
    }


    std::cout << "Renderizando...\n";
    my_gl::simpleRender(model1, textureImg, zbuffer, img, vec3f { 0, 0, 1 });
    std::cout << "Renderizacion hecha...\n";

    img.flip_vertically();
    img.write_tga_file("render6.tga");

    delete[] zbuffer;
    std::cout << "Hecho" << '\n';
}

// TODO:
/**
 * Refactorizar el código
 * hacer que el zbuffer se comparta para dos modelos que conforman uno solo
 */
