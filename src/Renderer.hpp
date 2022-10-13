#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <string>
#include "model.hpp"
#include "my_gl.hpp"
#include "math.hpp"

class Renderer {
public:
    Renderer(Model& model, TGAImage& context);
    ~Renderer();

    void render();
    void reInitialize(Model& model); 
    void doTransformation(const Matrix& mat,const vec3f& point);
    void generateImg(const std::string& filename);
    void clearImg();
    
    // It'll change (the definition)
    void setViewport(int x, int y, int w, int h);
    void setProyection(int z_distance);
    void setCamera(const vec3f& from, const vec3f& to, const vec3f& up);

    void setOutputImg(const TGAImage& img); 
    void setLightDirection(const vec3f& light_dir);
    void setModel(Model& model);

private:
    TGAImage& m_outputImg;
    float*    m_zbuffer;
    vec3f     m_lightDir;
    Model*    m_model;
    int m_height;
    int m_width;
    const int m_depth;

    Matrix proyection = Matrix::Identity(4); // Perspective proyection matrix
    Matrix viewport   = Matrix::Identity(4); // Viewport matrix
    Matrix modelView  = Matrix::Identity(4); // View matrix (with de lookat function)
};

#endif // RENDERER_H