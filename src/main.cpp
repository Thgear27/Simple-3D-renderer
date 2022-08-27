#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::ifstream file("obj/african_head/african_head.obj", std::ios::in);
    if (file.fail()) {
        std::cout << "Ha ocurrido un error al cargar el archivo" << std::endl;
    }
    if (file.fail()) {
        std::cout << "Ha ocurrido un error al cargar el archivo" << std::endl;
    }

    int ctn { 0 };
    char trash;
    std::string str;

    while (!file.fail()) {
        std::getline(file, str, '\n');
        ctn++;
    }

    std::cout << ctn << '\n';
}

// TODO:
/**
 * Leer un .obj y obtener los vertices y las faces
 * Dibujar una linea y triangulos
 * Dibujar las faces sin preocuparse del zbuffer
 */