#include "math.hpp"
#include <cmath>

Matrix vecToMat(const vec2f& vec) {
    std::cerr << "Ejecutado\n";
    Matrix mat { 3, 1 };
    mat[0] = { vec.x };
    mat[1] = { vec.y };
    mat[2] = { 1 };
    return mat;
}
Matrix vecToMat(const vec3f& vec) {
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