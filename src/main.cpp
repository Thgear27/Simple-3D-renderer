#include "Renderer.hpp"
#include "Timer.hpp"
#include "model.hpp"
#include "tgaimage.h"
#include "objpaths.hpp"
#include <iostream>

constexpr int height { 1200 };
constexpr int width  { 1200 };

int main() {
    TGAImage img { width, height, TGAImage::Format::RGB };
    Model modelo(diablo3pose, Model::Format::with_vt, "obj/rabbit/outTexture.tga", diablo3pose_nrmTangent);
    Model modelo2("obj/floor.obj", Model::Format::with_vt, "obj/floor_diffuse.tga", diablo3pose_nrmTangent);

    Renderer renderer(modelo, img);
    renderer.setLightDirection(vec3f { 0, 1, 0 });
    renderer.setViewport(0, 0, width, height);
    renderer.setProyection(4);
    renderer.setCamera(vec3f { 0.5, 0.5, 1 }, vec3f { 0, 0, 0 }, vec3f { 0, 1, 0 });

    Timer time;    
    time.reset();
    renderer.render();
    renderer.reInitialize(modelo2);
    renderer.setLightDirection(vec3f { 1, 1, 1 });
    renderer.render();
    std::cout << time.elapsed() << '\n';
    renderer.generateImg("SI13.tga");
    return 0;
}