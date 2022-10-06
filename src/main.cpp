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
    Model modelo(african_head, Model::Format::with_vt);

    Renderer renderer(modelo, img, "obj/rabbit/outTexture.tga");
    renderer.setLightDirection(vec3f {-1, 0, 1});

    Timer time;
    time.reset();

    renderer.render();
    
    double elaps = time.elapsed();
    std::cout << elaps << '\n';
    std::cout << "Renderizacion hecha...\n";

    renderer.generateImg("test2.tga");

    std::cout << "done..." << '\n';
    return 0;
}

// TODO:
/**
 * Refactorizar el código
 * hacer que el zbuffer se comparta para dos modelos que conforman uno solo
 */
