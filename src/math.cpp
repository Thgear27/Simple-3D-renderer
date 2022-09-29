#include "math.hpp"
#include <cmath>

vec3f toBarycentricCoord(vec3f* verts, vec2f point) {
    // Asumiendo r1, r2, r3 los vertices del triangulo, y p el punto
    // Variables que representan vectores matematicos
    vec3f r1_r2 = verts[1] - verts[0];
    vec3f r1_r3 = verts[2] - verts[0];
    vec3f p_r1 = verts[0] - (vec3f)point;

    // vec3f x_values { r1_r2.x, r1_r3.x, p_r1.x };
    // vec3f y_values { r1_r2.y, r1_r3.y, p_r1.y };
    vec3f vecRes = crossProduct(vec3f { r1_r2.x, r1_r3.x, p_r1.x }, vec3f { r1_r2.y, r1_r3.y, p_r1.y });

    if (std::abs(vecRes.z) < 1e-2)
        // if (std::abs(vecRes.z) < 1)
        return vec3f { -1, 1, 1 };

    vecRes = vecRes / vecRes.z;
    return (vec3f { 1.0f - (vecRes.x + vecRes.y), vecRes.x, vecRes.y });
}

vec3f toBarycentricCoord(vec2f* verts, vec2f point) {
    vec3f r1_r2 = verts[1] - verts[0];
    vec3f r1_r3 = verts[2] - verts[0];
    vec3f p_r1 = verts[0] - point;

    vec3f vecRes = crossProduct(vec3f { r1_r2.x, r1_r3.x, p_r1.x }, vec3f { r1_r2.y, r1_r3.y, p_r1.y });

    if (std::abs(vecRes.z) < 1e-2)
        // if (std::abs(vecRes.z) < 1)
        return vec3f { -1, 1, 1 };

    vecRes = vecRes / vecRes.z;
    return (vec3f { 1.0f - (vecRes.x + vecRes.y), vecRes.x, vecRes.y });
}

Matrix toHomogeneousCoord(const vec2f& vec) {
    std::cerr << "Ejecutado\n";
    Matrix mat { 3, 1 };
    mat[0] = { vec.x };
    mat[1] = { vec.y };
    mat[2] = { 1 };
    return mat;
}
Matrix toHomogeneousCoord(const vec3f& vec) {
    Matrix mat { 4, 1 };
    mat[0] = { vec.x };
    mat[1] = { vec.y };
    mat[2] = { vec.z };
    mat[3] = { 1 };
    return mat;
}

const vec2f matToVec2(const Matrix& mat) {
    if (mat.getNum_rows() != 3) std::cerr << "Error en matToVec2\n";
    return vec2f { mat[0][0] / mat[2][0], mat[1][0] / mat[2][0] };
}

const vec3f matToVec3(const Matrix& mat) {
    if (mat.getNum_rows() != 4) std::cerr << "Error en matToVec3\n";
    return vec3f { mat[0][0] / mat[3][0], mat[1][0] / mat[3][0], mat[2][0] / mat[3][0] };
}