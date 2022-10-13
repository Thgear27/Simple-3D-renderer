#include <limits>
#include "Renderer.hpp"

Renderer::Renderer(Model& model, TGAImage& context) 
    : m_outputImg{ context }, m_depth { 255 }
{
    m_height     = m_outputImg.get_height();
    m_width      = m_outputImg.get_width();

    m_model      = &model;
    m_zbuffer    = new float[m_height * m_width];
    m_lightDir   = vec3f{ 0, 0 ,1 };

    // zbuffer initialization
    for (int i = 0; i < m_height * m_width; i++) {
        m_zbuffer[i] = -std::numeric_limits<float>::max();
    }
}

Renderer::~Renderer() { delete[] m_zbuffer; }

void Renderer::render() {
    Matrix vpm = viewport * proyection * modelView;
    my_gl::simpleRender(*m_model, m_zbuffer, m_outputImg, m_lightDir, vpm);
}

void Renderer::reInitialize(Model& model) {
    m_model = &model;
}

void Renderer::doTransformation(const Matrix& mat,const vec3f& point) {
    vec3f* modelVecs;
    for (int i = 0; i < m_model->getTotalFaces(); i++) {
        modelVecs = m_model->getVertex_ptr(i);

        for (int vertexIndex = 0; vertexIndex < 3; vertexIndex++) {
            Matrix hmcoords = vecToMat(modelVecs[vertexIndex]);
            modelVecs[vertexIndex] = matToVec3(
                  my_gl::translate(point.x, point.y, point.z)
                * mat
                * my_gl::translate(-point.x, -point.y, -point.z)
                * hmcoords
            );
        }
    }
}

void Renderer::generateImg(const std::string& filename) {
    m_outputImg.flip_vertically();
    const char* cfilename = filename.c_str();
    m_outputImg.write_tga_file(cfilename);
}

void Renderer::clearImg() {
    m_outputImg.clear(); 
    for (int i = 0; i < m_height * m_width; i++) {
        m_zbuffer[i] = -std::numeric_limits<float>::max();
    }
}

void Renderer::setViewport(int x, int y, int w, int h) {
    viewport = Matrix::Identity(4);
    viewport[0][3] = x + w / 2.0f;
    viewport[1][3] = y + h / 2.0f;
    viewport[2][3] = m_depth / 2.0f;

    viewport[0][0] = w / 2.0f;
    viewport[1][1] = h / 2.0f;
    viewport[2][2] = m_depth / 2.0f;
}
void Renderer::setProyection(int z_distance) {
    proyection = my_gl::simpleProjection(z_distance);
}
void Renderer::lookAt(vec3f eye, vec3f center, vec3f up) {
    vec3f z = (eye - center);      z.normalize();
    vec3f x = crossProduct(up, z); x.normalize();
    vec3f y = crossProduct(z, x);  y.normalize();
    Matrix minv = Matrix::Identity(4);
    Matrix Tr = Matrix::Identity(4);
    minv[0] = { x.x, x.y, x.z, 0};
    minv[1] = { y.x, y.y, y.z, 0};
    minv[2] = { z.x, z.y, z.z, 0};
    Tr[0][3] = -eye.x;
    Tr[1][3] = -eye.y;
    Tr[2][3] = -eye.z;
    modelView = minv * Tr;
}

void Renderer::setOutputImg(const TGAImage& img) {
    m_outputImg = img; 
}
void Renderer::setLightDirection(const vec3f& light_dir) {
    m_lightDir = light_dir;
}
void Renderer::setModel(Model& model) {
    m_model = &model;
}