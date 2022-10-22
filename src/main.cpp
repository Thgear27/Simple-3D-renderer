#include "Renderer.hpp"
#include "Timer.hpp"
#include "model.hpp"
#include "tgaimage.h"
#include "objpaths.hpp"
#include <iostream>

constexpr int height { 1200 };
constexpr int width  { 1200 };

const char* whiteTexture = "obj/rabbit/outTexture.tga";

int main() {
    TGAImage img { width, height, TGAImage::Format::RGB };
    // Model modelo2(african_head, Model::Format::with_vt, african_head_texture_img, african_head_nm_tangent);
    // Model modelo2("obj/floor.obj", Model::Format::with_vt, "obj/floor_diffuse.tga", "obj/floor_nm_tangent.tga");
    Model modelo2(floor_obj, Model::Format::with_vt, "obj/floor_diffuse.tga", african_head_nm_tangent);
    Renderer renderer(modelo2, img);
    renderer.setLightDirection(vec3f { 0, -1, 1 });
    renderer.setViewport(0, 0, width, height);
    renderer.setProyection(4);
    renderer.setCamera(vec3f { 0, 0.4, 0.1 }, vec3f { 0, 0, 0 }, vec3f { 0, 1, 0 });

    Timer time;    
    time.reset();
    renderer.render();
    // renderer.reInitialize(modelo2);
    // renderer.setLightDirection(vec3f { 1, 1, 1 });
    // renderer.render();
    std::cout << time.elapsed() << '\n';
    renderer.generateImg("SI15.tga");

    return 0;
}