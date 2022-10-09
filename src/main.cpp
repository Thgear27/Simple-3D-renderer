#include "Renderer.hpp"
#include "Timer.hpp"
#include "model.hpp"
#include "tgaimage.h"
#include "objpaths.hpp"
#include <iostream>

constexpr int height { 1000 };
constexpr int width  { 1000 };

/**
 * TODO:
 * Añadir la componente z en la funcion doTransformation para determinara desde donde se hara la tranformacion
 * Hacer que la proyección se añada al final y no modifique las coordenadas reales
 * Hacer la funcion viewport (que devuelva la matrix de trasformacion)
 * Hacer la funcion lookAt
 */

int main() {
    TGAImage img { width, height, TGAImage::Format::RGB };
    Model modelo(cubePortal, Model::Format::with_vt);

    Renderer renderer(modelo, img, cubePortalTexture_img);
    renderer.setLightDirection(vec3f { -1, -1, 1 });
    renderer.setViewport();
    
    renderer.doTransformation(my_gl::zoom(0.4f), vec3f{ width / 2, height / 2, 0 });
    renderer.doTransformation(my_gl::rotatey(M_PI/4), modelo.getModelCenter());
    renderer.doTransformation(my_gl::rotatex(-M_PI/4), modelo.getModelCenter());
    renderer.doTransformation(my_gl::simpleProjection(800), vec3f{ width / 2, height / 2, 0 });

    renderer.render();
    renderer.clearImg();
    renderer.render();
    
    std::string file = "hola" + std::to_string(2) + ".tga";
    std::cout << modelo.getModelCenter() << '\n';

    renderer.generateImg(file);

    std::cout << "done..." << '\n';
    return 0;
}