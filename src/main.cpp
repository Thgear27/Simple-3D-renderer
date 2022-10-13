#include "Renderer.hpp"
#include "Timer.hpp"
#include "model.hpp"
#include "tgaimage.h"
#include "objpaths.hpp"
#include <iostream>

constexpr int height { 1000 };
constexpr int width  { 1000 };

/**
 * Hacer que la proyección se añada al final y no modifique las coordenadas reales
 * Hacer la funcion viewport (que devuelva la matrix de trasformacion)
 * Hacer la funcion lookAt
 */

int main() {
    TGAImage img { width, height, TGAImage::Format::RGB };
    Model modelo(diablo3pose, Model::Format::with_vt, "obj/rabbit/outTexture.tga");

    Renderer renderer(modelo, img);
    renderer.setLightDirection(vec3f { 0, 0, -1 });
    renderer.setViewport(0, 0, width, height);
    renderer.setProyection(10);
    renderer.lookAt(vec3f { -1, 0.01, -2 }, vec3f { 0, 0.1, 0 }, vec3f { 0, 1, 0 });
    

    renderer.render();
    renderer.generateImg("SI.tga");

    std::cout << "done..." << '\n';
    return 0;
}