#include "math.hpp"
#include <iostream>

int main() {
    vec2f v1 { 1.01, 2.21 };
    vec2f v2 { 6.22, 2.21 };

    std::cout << crossProduct( static_cast<vec3f>(v1), static_cast<vec3f>(v2)) << '\n';
}

// TODO:
/**
 * Leer un .obj y obtener los vertices y las faces
 * Dibujar una linea y triangulos
 * Dibujar las faces sin preocuparse del zbuffer
 */