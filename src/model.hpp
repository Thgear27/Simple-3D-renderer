#ifndef MODEL_H
#define MODEL_H

#include "math.hpp"
#include "tgaimage.h"
#include <vector>

struct face {
    int v_idx[3] {};
    int vt_idx[3] {};
    int vn_idx[3] {};
};

class Model {
public:
    enum Format { no_vt, with_vt };

    Model(const char* filename, Format l_format, const char* textureImgFile);
    ~Model();

    int getTotalFaces() { return total_faces; }
    int getTotalVertices() { return total_v; }
    int getTotalVertsTextures() { return total_vt; }
    int getTotalVertsNormals() { return total_vn; }
    Format getFormat() { return format; }

    bool load_model_from_file(const char* filename);
    TGAColor diffuse(const vec2f& uv_coord);

    // Member functions added
    vec3f getModelCenter();

    vec3f* getVertex_ptr(int face_index);
    vec3f* getVertexNormal_ptr(int face_index);
    vec2f* getVertexTexture_ptr(int face_index);

    TGAImage& getTextureImg();
private:
    Format format;

    int total_v { 0 };
    int total_vt { 0 };
    int total_vn { 0 };
    int total_faces { 0 };

    vec3f* v_ptr  { nullptr };
    vec2f* vt_ptr { nullptr };
    vec3f* vn_ptr { nullptr };
    
    vec3f& getVertex(int face_index, int which_vertex);
    vec3f& getVertexNormal(int face_index, int which_vertex);
    vec2f& getVertexTexture(int face_index, int which_vertex);

    std::vector<vec3f> vertices;
    std::vector<vec2f> vertices_textures;
    std::vector<vec3f> vertices_normals;
    std::vector<face> faces;

    TGAImage m_textureImg;
};

#endif // MODEL_H