#include "model.hpp"
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

Model::Model(const char* filename) { load_model_from_file(filename); }

Model::~Model() {}

bool Model::load_model_from_file(const char* filename) {
    std::ifstream model_file(filename);

    if (!model_file.is_open()) {
        std::cerr << "No se puede cargar el archivo :(\n";
        return false;
    }

    std::string line;
    char trash;

    while (model_file.good()) {
        std::stringstream line_stream;
        std::getline(model_file, line, '\n');
        line_stream.str(line);

        if (!line.compare(0, 2, "v ")) {
            line_stream >> trash;

            vec3f vertex;
            line_stream >> vertex.x;
            line_stream >> vertex.y;
            line_stream >> vertex.z;

            // std::cout << vertex << '\n';

            vertices.push_back(vertex);
            total_v++;

        } else if (!line.compare(0, 3, "vt ")) {
            line_stream >> trash >> trash;

            vec2f vertex_texture;
            line_stream >> vertex_texture.x;
            line_stream >> vertex_texture.y;

            // std::cout << vertex_texture << '\n';

            vertices_textures.push_back(vertex_texture);
            total_vt++;

        } else if (!line.compare(0, 3, "vn ")) {
            line_stream >> trash >> trash;

            vec3f vertex_normal;
            line_stream >> vertex_normal.x;
            line_stream >> vertex_normal.y;
            line_stream >> vertex_normal.z;

            // std::cout << vertex_normal << '\n';

            vertices_normals.push_back(vertex_normal);
            total_vn++;

        } else if (!line.compare(0, 2, "f ")) {
            line_stream >> trash;

            face temp_face;
            for (int i = 0; i < 3; i++) {
                line_stream >> temp_face.v_idx[i];
                line_stream >> trash;
                line_stream >> temp_face.vt_idx[i];
                line_stream >> trash;
                line_stream >> temp_face.vn_idx[i];

                temp_face.v_idx[i]--;
                temp_face.vt_idx[i]--;
                temp_face.vn_idx[i]--;
            }
            faces.push_back(temp_face);
            total_faces++;
        }
    }
    std::cout << "Archivo: " << filename << " ha sido cargado\n";
    std::cout << "Datos regodidos:\n";
    std::cout << "\t#vertices:\t\t" << total_v << '\n';
    std::cout << "\t#vertices textures:\t" << total_vt << '\n';
    std::cout << "\t#vertices normals:\t" << total_vn << '\n';
    std::cout << "\t#faces:\t\t\t" << total_faces << '\n';
    return true;
}

vec3f Model::getVertex(int face_index, int which_vertex) {
    if (!(which_vertex <= 3 && which_vertex >= 1))
        std::cout << "parametro wich_vertex tiene que ser de 1 a 3\n";
    assert(which_vertex <= 3 && which_vertex >= 1);
    return vertices[faces[face_index].v_idx[which_vertex - 1]];
}