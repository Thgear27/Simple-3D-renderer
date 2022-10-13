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
    Model modelo(cubePortal, Model::Format::with_vt, cubePortalTexture_img);

    Renderer renderer(modelo, img);
    renderer.setLightDirection(vec3f { -1, 5, 1 });
    renderer.setViewport(0, 0, width, height);
    renderer.setProyection(10);
    renderer.setCamera(vec3f { -5, 5, 5 }, vec3f { 0, 0.1, 0 }, vec3f { 0, 1, 0 });
    
    renderer.render();
    renderer.generateImg("SI.tga");

    std::cout << "done..." << '\n';
    return 0;
}