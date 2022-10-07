#include <limits>
#include "Renderer.hpp"

Renderer::Renderer(Model& model, TGAImage& context, const char* texture) 
    : m_outputImg{ context }
{
    m_height     = m_outputImg.get_height();
    m_width      = m_outputImg.get_width();

    m_model      = &model;
    m_textureImg.read_tga_file(texture);
    m_textureImg.flip_vertically();
    m_zbuffer    = new float[m_height * m_width];
    m_lightDir   = vec3f{ 0, 0 ,1 };

    // zbuffer initialization
    for (int i = 0; i < m_height * m_width; i++) {
        m_zbuffer[i] = -std::numeric_limits<float>::max();
    }
    
}

Renderer::~Renderer() { delete[] m_zbuffer; }

void Renderer::render() {
    my_gl::simpleRender(*m_model, m_textureImg, m_zbuffer, m_outputImg, m_lightDir);
}

void Renderer::reInitialize(Model& model, const char* texture) {
    m_model      = &model;
    m_textureImg.read_tga_file(texture);
    m_textureImg.flip_vertically();
}

void Renderer::doTransformation(const Matrix& mat, int x, int y) {
    vec3f* modelVecs = new vec3f[3] {};
    for (int i = 0; i < m_model->getTotalFaces(); i++) {
        modelVecs = m_model->getVertex_ptr(i);

        for (int vertexIndex = 0; vertexIndex < 3; vertexIndex++) {
            Matrix hmcoords = vecToMat(modelVecs[vertexIndex]);
            modelVecs[vertexIndex] = matToVec3(
                  my_gl::translate(x, y, 0)
                * mat
                * my_gl::translate(-x, -y, 0)
                * hmcoords
            );
        }
    }
}

void Renderer::setViewport() {
    vec3f* modelVecs = new vec3f[3] {};
    for (int i = 0; i < m_model->getTotalFaces(); i++) {
        modelVecs = m_model->getVertex_ptr(i);
        for (int vertexIndex = 0; vertexIndex < 3; vertexIndex++) {
            modelVecs[vertexIndex].x = (modelVecs[vertexIndex].x + 1.0f)  * m_width  / 2;
            modelVecs[vertexIndex].y = (modelVecs[vertexIndex].y + 1.0f)  * m_height / 2;
            modelVecs[vertexIndex].z = (modelVecs[vertexIndex].z + 1.0f)  * 255;
        }
    }
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