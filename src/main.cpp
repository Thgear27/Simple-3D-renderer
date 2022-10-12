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
    Model modelo(cubePortal, Model::Format::with_vt);

    Renderer renderer(modelo, img, cubePortalTexture_img);
    renderer.setLightDirection(vec3f { 1, -1, 1 });
    renderer.setViewport();
    

    renderer.doTransformation(my_gl::translate(0, 0, -900), modelo.getModelCenter());

    renderer.doTransformation(my_gl::rotatey(0.2f), modelo.getModelCenter());
    renderer.doTransformation(my_gl::rotatex(0.2f), modelo.getModelCenter());
    renderer.render(1000);
    renderer.generateImg("SI.tga");

    std::cout << "done..." << '\n';
    return 0;
}