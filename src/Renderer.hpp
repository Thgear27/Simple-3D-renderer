#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <string>
#include "model.hpp"
#include "my_gl.hpp"
#include "math.hpp"

class Renderer {
public:
    Renderer(Model& model, TGAImage& context, const char* texture);
    ~Renderer();

    void render();
    void reInitialize(Model& model, const char* texture); 
    void doTransformation(const Matrix& mat,const vec3f& point);
    void generateImg(const std::string& filename);
    void clearImg();
    
    // It'll change (the definition)
    void setViewport();

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
    int m_height;
    int m_width;
};

#endif // RENDERER_H