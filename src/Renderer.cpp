#include <limits>
#include "Renderer.hpp"

//////////////////////////////////////////////////////////////
class GouraudShader : public my_gl::shader_i {
    const Matrix& m_viewport;
    const Matrix& m_proyection;
    const Matrix& m_modelView;
    const vec3f&  m_lightDir;
    vec3f* vecNormals;
    Model*  m_model;
    float intensity = 0.0f;

    GouraudShader(const Matrix& v, const Matrix& p, const Matrix& m, const vec3f& lightDir)
    : m_viewport(v), m_proyection(p), m_modelView(m), m_lightDir(lightDir) {}

    vec3f vertex(int i_face, int which_vertex) override {
        vecNormals = m_model->getVertexNormal_ptr(i_face);
        vec3f vec = m_model->getVertex_ptr(i_face)[which_vertex];
        return matToVec3(m_viewport * m_proyection * m_modelView * vecToMat(vec));
    }
    bool fragment(const vec3f& bar, TGAColor& color) override {
        vec3f normalResult = 
            vecNormals[0] * bar.x + vecNormals[1] * bar.y + vecNormals[2] * bar.z;
        intensity = std::max(0.0f, dotProduct(normalResult, m_lightDir));
        return false;
    }
};
//////////////////////////////////////////////////////////////

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
    viewport = my_gl::viewport(x, y, w, h, m_depth);
}
void Renderer::setProyection(int z_distance) {
    proyection = my_gl::simpleProjection(z_distance);
}
void Renderer::setCamera(const vec3f& from, const vec3f& to, const vec3f& up) {
    modelView = my_gl::lookAt(from, to, up);
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