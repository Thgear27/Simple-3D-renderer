#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "model.hpp"
#include "my_gl.hpp"
#include "math.hpp"

class Renderer {
public:
    Renderer(Model& model, TGAImage& context, const char* texture);
    ~Renderer();

    void render();
    void reInitialize(Model& model, const char* texture); 
    void doTransformation(Matrix mat);
    void generateImg(const char* filename);

    void setOutputImg(const TGAImage& img); 
    void setTextureImg(const TGAImage& img);
    void setLightDirection(const vec3f& light_dir);
    void setModel(Model& model);

private:
    TGAImage& m_outputImg;
    TGAImage  m_textureImg;
    float*    m_zbuffer;
    vec3f     m_lightDir;
    Model*    m_model;
};

#endif // RENDERER_H