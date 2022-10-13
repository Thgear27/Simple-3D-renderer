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
void Renderer::lookAt(vec3f from, vec3f to, vec3f up) {
    vec3f foward = (from - to);      foward.normalize();
    vec3f right = crossProduct(up, foward); right.normalize();
    vec3f newup = crossProduct(foward, right);  newup.normalize();
    Matrix minv = Matrix::Identity(4);
    Matrix traslation = Matrix::Identity(4);
    minv[0] = { right.x,  right.y,  right.z,  0 };
    minv[1] = { newup.x,  newup.y,  newup.z,  0 };
    minv[2] = { foward.x, foward.y, foward.z, 0 };
    traslation[0][3] = -from.x;
    traslation[1][3] = -from.y;
    traslation[2][3] = -from.z;
    modelView = minv * traslation;
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