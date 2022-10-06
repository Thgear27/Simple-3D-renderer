#include <limits>
#include "Renderer.hpp"

Renderer::Renderer(Model& model, TGAImage& context, TGAImage& texture) 
    : m_outputImg{ context }
{
    int height     = m_outputImg.get_height();
    int width      = m_outputImg.get_width();

    m_model      = &model;
    m_textureImg = texture;
    m_zbuffer    = new float[height * width];
    m_lightDir   = vec3f{ 0, 0 ,1 };

    // zbuffer initialization
    for (int i = 0; i < height * width; i++) {
        m_zbuffer[i] = -std::numeric_limits<float>::max();
    }
    
}

Renderer::~Renderer() { delete[] m_zbuffer; }

void Renderer::render() {
    my_gl::simpleRender(*m_model, m_textureImg, m_zbuffer, m_outputImg, m_lightDir);
}

void Renderer::reInitialize(Model& model, TGAImage& texture) {
    m_model      = &model;
    m_textureImg = texture;
}

void Renderer::doTransformation(Matrix mat) {
    // TODO
}

void Renderer::generateImg(const char* filename) {
    m_outputImg.flip_vertically();
    m_outputImg.write_tga_file(filename);
}

void Renderer::setOutputImg(const TGAImage& img) {
    m_outputImg = img; 
}
void Renderer::setTextureImg(const TGAImage& img) {
    m_textureImg = img;
}
void Renderer::setLightDirection(const vec3f& light_dir) {
    m_lightDir = light_dir;
}
void Renderer::setModel(Model& model) {
    m_model = &model;
}