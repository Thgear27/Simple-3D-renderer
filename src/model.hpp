#ifndef MODEL_H
#define MODEL_H

#include "math.hpp"
#include <vector>

struct face {
    int v_idx[3] {};
    int vt_idx[3] {};
    int vn_idx[3] {};
};

class Model {
public:
    Model() = default;
    Model(const char* filename);
    ~Model();

    int getTotalFaces() { return total_faces; }
    int getTotalVertices() { return total_v; }
    int getTotalVertsTextures() { return total_vt; }
    int getTotalVertsNormals() { return total_vn; }

    bool load_model_from_file(const char* filename);

    // Member functions added

    vec3f getVertex(int face_index, int which_vertex);
    vec3f getVertexNormal(int face_index, int which_vertex);
    vec3f getVertexTexture(int face_index, int which_vertex);

private:
    int total_v { 0 };
    int total_vt { 0 };
    int total_vn { 0 };
    int total_faces { 0 };

    std::vector<vec3f> vertices;
    std::vector<vec2f> vertices_textures;
    std::vector<vec3f> vertices_normals;
    std::vector<face> faces;
};

#endif // MODEL_H