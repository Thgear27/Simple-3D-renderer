#include "math.hpp"
#include "model.hpp"
#include <fstream>
#include <iostream>

int main() {
    Model modelo("obj/african_head/african_head.obj");
    std::cout << modelo.getVertex(0, 3) << '\n';
}

// TODO:
/**
 * Dibujar una linea y triangulos
 * Dibujar las faces sin preocuparse del zbuffer
 */