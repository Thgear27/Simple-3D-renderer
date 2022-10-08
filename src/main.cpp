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
 * Hacer la funcion viewport (que devuelva la matrix de trasformacion)
 * Hacer la funcion lookAt
 */

int main() {
    TGAImage img { width, height, TGAImage::Format::RGB };
    Model modelo(boogie_body, Model::Format::with_vt);

    Renderer renderer(modelo, img, boogie_body_texture_img);
    renderer.setLightDirection(vec3f {-1, 0, 1});
    renderer.setViewport();
    
    renderer.doTransformation(my_gl::zoom(0.6f), width / 2, height / 2);
    renderer.doTransformation(my_gl::rotatey(1.2f), 0, 0);
    renderer.doTransformation(my_gl::simpleProjection(800), width / 2, height / 2);

    renderer.render();
    renderer.clearImg();
    renderer.render();
    
    std::string file = "hola" + std::to_string(1) + ".tga";

    renderer.generateImg(file);

    std::cout << "done..." << '\n';
    return 0;
}