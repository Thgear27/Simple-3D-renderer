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

    Matrix vertex(int i_face, int which_vertex) override {
        vecNormals = m_model->getVertexNormal_ptr(i_face);
        vec3f vec = m_model->getVertex_ptr(i_face)[which_vertex];
        return m_viewport * m_proyection * m_modelView * vecToMat(vec);
    }

    bool fragment(const vec3f& bar, TGAColor& color, vec3f* verts) override {
        vec3f normalResult = vecNormals[0] * bar.x + vecNormals[1] * bar.y + vecNormals[2] * bar.z;
        intensity = std::max(0.0f, dotProduct(normalResult, m_lightDir));
        for (int i = 0; i < 3; i++) {
            color.raw[i] = 255 * intensity;
        }
        return false;
        verts;
    }
    ~GouraudShader() override { std::cout << "called\n"; }
};

struct textureShader : public my_gl::shader_i {
    const Matrix& m_viewport;
    const Matrix& m_proyection;
    const Matrix& m_modelView;
    const vec3f&  m_lightDir;

    Matrix uv_mat { 2, 3 }; // coordenadas uv
    Matrix nrm_mat { 3, 3 }; // Normal per vertex
    // Matrix ndc_tri { 3, 3 }; // Normalized device coodinates

    Model* m_model;
    TGAImage& m_textureImg;
    float intensity = 0.0f;

    textureShader(const Matrix& v, const Matrix& p, const Matrix& m, const vec3f& lightDir, Model* model, TGAImage& textureImg)
    : m_viewport(v), m_proyection(p), m_modelView(m), m_lightDir(lightDir), m_textureImg(textureImg) {
        m_model = model;
    }

    Matrix vertex(int i_face, int which_vertex) override {
        // THIS
        nrm_mat.setCol(which_vertex, m_model->getVertexNormal_ptr(i_face)[which_vertex]);
        uv_mat.setCol(which_vertex, m_model->getVertexTexture_ptr(i_face)[which_vertex]);

        vec3f gl_vertex = m_model->getVertex_ptr(i_face)[which_vertex];
        Matrix ret = m_proyection * m_modelView * vecToMat(gl_vertex);

        return m_viewport * ret;
    }

    bool fragment(const vec3f& bar, TGAColor& color, vec3f* verts) override {
        vec3f normalRes = mult3x3(nrm_mat, bar);
        vec2f uv        = mult2x3(uv_mat, bar);

        normalRes.normalize();

        Matrix A { 3, 3 };
        A[0] = { verts[1].x - verts[0].x, verts[1].y - verts[0].y, verts[1].z - verts[0].z }; 
        A[1] = { verts[2].x - verts[0].x, verts[2].y - verts[0].y, verts[2].z - verts[0].z };
        A[2] = { normalRes.x, normalRes.y, normalRes.z };

        A.inverse();

        vec3f i = mult3x3(A, vec3f{ uv_mat[0][1] - uv_mat[0][0], uv_mat[0][2] - uv_mat[0][0], 0 });
        vec3f j = mult3x3(A, vec3f{ uv_mat[1][1] - uv_mat[1][0], uv_mat[1][2] - uv_mat[1][0], 0 });
        i.normalize();
        j.normalize();

        Matrix B { 3, 3 };
        B.setCol(0, i);
        B.setCol(1, j);
        B.setCol(2, normalRes);

        vec3f n = mult3x3(B, m_model->normal(uv));
        n.normalize();

        vec3f r = (n * (dotProduct(n, m_lightDir) * 2.0f)) - m_lightDir;
        r.normalize();
        float exponent = m_model->specular(uv);
        if (exponent == 0) exponent = 1.0f;
        float spec = std::pow(std::max(r.z, 0.0f), exponent);

        intensity = std::max(0.0f, dotProduct(n, m_lightDir));

        for (int i = 0; i < 3; i++) {
            color.raw[i] = std::min<float>(255, (m_model->diffuse(uv).raw[i] * (intensity + 0.6*spec)));
        }
        
        // color = m_model->diffuse(uv) * intensity;
        return false;
    }
    ~textureShader() override { std::cout << "called\n"; }
};
//////////////////////////////////////////////////////////////

Renderer::Renderer(Model& model, TGAImage& context) 
    : m_outputImg{ context }, m_depth { 255 }
{
    m_height   = m_outputImg.get_height();
    m_width    = m_outputImg.get_width();

    m_model    = &model;
    m_zbuffer  = new float[m_height * m_width];
    m_lightDir = vec3f{ 0, 0 ,1 };

    // zbuffer initialization
    for (int i = 0; i < m_height * m_width; i++) {
        m_zbuffer[i] = -std::numeric_limits<float>::max();
    }
}

Renderer::~Renderer() { delete[] m_zbuffer; }

void Renderer::render() {
    m_lightDir.normalize();
    textureShader shader { viewport, proyection, modelView, m_lightDir, m_model, m_model->getTextureImg() };
    // GouraudShader shader { viewport, proyection, modelView, m_lightDir, m_model };

    Matrix* screen_coords = new Matrix[3] {};
    for (int i = 0; i < m_model->getTotalFaces(); i++) {
        for (int vi = 0; vi < 3; vi++) {
            screen_coords[vi] = shader.vertex(i, vi);
        }
        my_gl::triangle(screen_coords, m_zbuffer, m_outputImg, shader);
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