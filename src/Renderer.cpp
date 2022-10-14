#include <limits>
#include "Renderer.hpp"

//////////////////////////////////////////////////////////////
struct GouraudShader : public my_gl::shader_i {
    const Matrix& m_viewport;
    const Matrix& m_proyection;
    const Matrix& m_modelView;
    const vec3f&  m_lightDir;
    vec3f* vecNormals;
    Model*  m_model;
    float intensity = 0.0f;

    GouraudShader(const Matrix& v, const Matrix& p, const Matrix& m, const vec3f& lightDir, Model* model)
    : m_viewport(v), m_proyection(p), m_modelView(m), m_lightDir(lightDir) {
        m_model = model;
    }

    vec3f vertex(int i_face, int which_vertex) override {
        vecNormals = m_model->getVertexNormal_ptr(i_face);
        vec3f vec = m_model->getVertex_ptr(i_face)[which_vertex];
        return matToVec3(m_viewport * m_proyection * m_modelView * vecToMat(vec));
    }
    bool fragment(const vec3f& bar, TGAColor& color) override {
        vec3f normalResult = 
            vecNormals[0] * bar.x + vecNormals[1] * bar.y + vecNormals[2] * bar.z;
        intensity = std::max(0.0f, dotProduct(normalResult, m_lightDir));
        for (int i = 0; i < 3; i++) {
            uint8_t c = static_cast<uint8_t>(intensity * 255);
            color = TGAColor{ c, c, c, 255 };
        }
        return false;
    }
    ~GouraudShader() override { std::cout << "called\n"; }
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
    GouraudShader shader { viewport, proyection, modelView, m_lightDir, m_model };

    m_lightDir.normalize();
    vec2f mvc[3] = { vec2f { 0.0f, 0.0f }, vec2f { 0.9f, 0.9f }, vec2f { 0.0f, 0.9f }};
    vec2f* modelVtCoords; 
    vec3f* screen_coords = new vec3f[3] {};
    vec3f* modelVecNormals;
    if (m_model->getFormat() == Model::Format::no_vt) modelVtCoords = mvc;
    for (int i = 0; i < m_model->getTotalFaces(); i++) {
        if (m_model->getFormat() == Model::Format::with_vt)
            modelVtCoords = m_model->getVertexTexture_ptr(i);

        modelVecNormals = m_model->getVertexNormal_ptr(i);

        for (int vi = 0; vi < 3; vi++) {
            screen_coords[vi] = shader.vertex(i, vi);
            std::cout << screen_coords[vi] << '\n';
        }

        // la nueva función se le tiene que ingresar el shader
        my_gl::triangle(screen_coords, m_zbuffer, m_model->getTextureImg(), modelVtCoords, m_outputImg, modelVecNormals, m_lightDir, true);
    }
    delete[] screen_coords;
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