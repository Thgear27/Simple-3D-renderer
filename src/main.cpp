#include "model.hpp"
#include "my_gl.hpp"
#include "tgaimage.h"
#include "matrix.hpp"
#include <fstream>
#include <iostream>

constexpr int height { 500 };
constexpr int width  { 500 };

const char* african_head             = "obj/african_head/african_head.obj";
const char* african_head_texture_img = "obj/african_head/african_head_diffuse.tga";

const char* ae_inner             = "obj/african_head/african_head_eye_inner.obj";
const char* ae_inner_texture_img = "obj/african_head/african_head_eye_inner_diffuse.tga";

const char* ae_outer             = "obj/african_head/african_head_eye_outer.obj";
const char* ae_outer_texture_img = "obj/african_head/african_head_eye_outer_diffuse.tga";

const char* boogie_head             = "obj/boggie/head.obj";
const char* boogie_head_texture_img = "obj/boggie/head_diffuse.tga";

const char* boogie_body             = "obj/boggie/body.obj";
const char* boogie_body_texture_img = "obj/boggie/body_diffuse.tga";

const char* diablo3pose             = "obj/diablo3_pose/diablo3_pose.obj";
const char* diablo3pose_texture_img = "obj/diablo3_pose/diablo3_pose_diffuse.tga";

int main() {
    TGAImage img { width, height, TGAImage::Format::RGB };
    Model modelo(diablo3pose, Model::Format::with_vt);

    TGAImage textureImg {};
    textureImg.read_tga_file(diablo3pose_texture_img);
    textureImg.flip_vertically();

    float* zbuffer = new float[width * height];
    for (int i = 0; i < width * height; i++) {
        zbuffer[i] = -std::numeric_limits<float>::max();
    }

    my_gl::simpleRender(modelo, textureImg, zbuffer, img, vec3f { 0, 0, 1 });
    std::cout << "Renderizacion hecha...\n";

    img.flip_vertically();
    img.write_tga_file("test5.tga");
    std::cout << "done..." << '\n';
    return 0;
}

// TODO:
/**
 * Refactorizar el código
 * hacer que el zbuffer se comparta para dos modelos que conforman uno solo
 */
