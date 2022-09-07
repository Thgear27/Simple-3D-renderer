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
    Model model1 { african_head };
    for (int i = 0; i < model1.getTotalFaces(); i++) {
        vec3f vertex1 = model1.getVertex(i, 1);
        vec3f vertex2 = model1.getVertex(i, 2);
        vec3f vertex3 = model1.getVertex(i, 3);

        vertex1.x = (vertex1.x + 1.0f) * img.get_width() / 2;
        vertex1.y = (vertex1.y + 1.0f) * img.get_height() / 2;
        vertex2.x = (vertex2.x + 1.0f) * img.get_width() / 2;
        vertex2.y = (vertex2.y + 1.0f) * img.get_height() / 2;
        vertex3.x = (vertex3.x + 1.0f) * img.get_width() / 2;
        vertex3.y = (vertex3.y + 1.0f) * img.get_height() / 2;

        vec3f verts[] { vertex1, vertex2, vertex3 };
        my_gl::triangle(verts, img, TGAColor { rand() % 255, rand() % 255, rand() % 255, 255 });
        // my_gl::triangle(verts, img, color::white);
    }
    img.flip_vertically();
    img.write_tga_file("test.tga");
    std::cout << "hecho" << '\n';
}

// TODO:
/**
 * Refactorizar el código
 * Dibujar (rellenarlas) las faces sin preocuparse del zbuffer
 */