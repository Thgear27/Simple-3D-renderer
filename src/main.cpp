#include "Renderer.hpp"
#include "Timer.hpp"
#include "model.hpp"
#include "tgaimage.h"
#include "objpaths.hpp"
#include <iostream>

constexpr int height { 1000 };
constexpr int width  { 1000 };

int main() {
    TGAImage img { width, height, TGAImage::Format::RGB };
    Model modelo(african_head, Model::Format::with_vt, african_head_texture_img);
    Model modelo2("obj/floor.obj", Model::Format::with_vt, "obj/floor_diffuse.tga");

    Renderer renderer(modelo, img);
    renderer.setLightDirection(vec3f { 1, 0, 1 });
    renderer.setViewport(0, 0, width, height);
    renderer.setProyection(4);
    renderer.setCamera(vec3f { 1, 0.2, 1 }, vec3f { 0, 0, 0 }, vec3f { 0, 1, 0 });

    Timer time;    
    time.reset();
    renderer.render();
    renderer.reInitialize(modelo2);
    renderer.setLightDirection(vec3f { 1, 1, 1 });
    renderer.render();
    std::cout << time.elapsed() << '\n';
    renderer.generateImg("SI13.tga");

    std::cout << "done..." << '\n';
    return 0;
}