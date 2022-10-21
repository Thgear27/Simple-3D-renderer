#include "model.hpp"
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

Model::Model(const char* filename, Format l_format, const char* textureImgFile, const char* nrmFile) 
    : format(l_format) 
{
    if (load_model_from_file(filename)) {
        v_ptr  = new vec3f[total_faces * 3];
        vt_ptr = new vec2f[total_faces * 3];
        vn_ptr = new vec3f[total_faces * 3];
        for (int face_i = 0; face_i < total_faces; face_i++) {
            for (int i = 0; i < 3; i++) {
                v_ptr[3 * face_i + i] = getVertex(face_i, i + 1);
            }
            for (int i = 0; i < 3; i++) {
                vt_ptr[3 * face_i + i] = getVt(face_i, i + 1);
            }
            for (int i = 0; i < 3; i++) {
                vn_ptr[3 * face_i + i] = getVertexNormal(face_i, i + 1);
            }
        }
    } 
    m_normalMap.read_tga_file(nrmFile);
    m_normalMap.flip_vertically();
    m_textureImg.read_tga_file(textureImgFile);
    m_textureImg.flip_vertically();
}

Model::~Model() {
    delete[] v_ptr;
    delete[] vt_ptr;
    delete[] vn_ptr;
}

TGAImage& Model::getTextureImg() {
    return m_textureImg;
}

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
                
                if (format == Format::with_vt) {
                    line_stream >> temp_face.vt_idx[i];
                }

                line_stream >> trash;
                line_stream >> temp_face.vn_idx[i];

                temp_face.v_idx[i]--;
                temp_face.vt_idx[i]--;
                temp_face.vn_idx[i]--;
            }
            if (line_stream.fail()) {
                std::cerr << "Ya nos cargo la verga\n";
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

TGAColor Model::diffuse(const vec2f& uv) {
    return m_textureImg.get(uv.x * m_textureImg.get_width(), uv.y * m_textureImg.get_height());
}

vec3f Model::normal(const vec2f& uv_coord) {
    vec3f ret;
    vec2f point;
    point.x = uv_coord.x * m_normalMap.get_width();
    point.y = uv_coord.y * m_normalMap.get_height();
    TGAColor color = m_normalMap.get((int)point.x, (int)point.y);
    ret.x = (float)color.raw[0] / 255.0f * 2.0f - 1.0f;
    ret.y = (float)color.raw[1] / 255.0f * 2.0f - 1.0f;
    ret.z = (float)color.raw[2] / 255.0f * 2.0f - 1.0f;
    return ret; 
}

vec3f& Model::getVertex(int face_index, int which_vertex) {
    return vertices[faces[face_index].v_idx[which_vertex - 1]];
}

vec3f& Model::getVertexNormal(int face_index, int which_vertex) {
    return vertices_normals[faces[face_index].vn_idx[which_vertex - 1]];
}

vec2f& Model::getVt(int face_index, int which_vertex) {
    return vertices_textures[faces[face_index].vt_idx[which_vertex - 1]];
}

// For performance reasons
vec3f* Model::getVertex_ptr(int face_index) {
    return v_ptr + (face_index * 3);
}
vec3f* Model::getVertexNormal_ptr(int face_index) {
    return vn_ptr  + (face_index * 3);
}
vec2f* Model::getVertexTexture_ptr(int face_index) {
    return vt_ptr  + (face_index * 3);
}

vec3f Model::getModelCenter() {
    vec3f center { 0, 0, 0 };
    for (int i = 0; i < total_faces; i++) {
        for (int j = 0; j < 3; j++) {
            center = center + getVertex_ptr(i)[j];
        }
    }
    return center / (float)(total_faces * 3);
}