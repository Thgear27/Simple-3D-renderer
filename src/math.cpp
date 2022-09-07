#include "math.hpp"
#include <cmath>

vec3f toBarycentricCoord(vec3f* verts, vec2f point) {
    // Asumiendo r1, r2, r3 los vertices del triangulo, y p el punto
    // Variables que representan vectores matematicos
    vec2f r1_r2 = discard_Z(verts[1] - verts[0]);
    vec2f r1_r3 = discard_Z(verts[2] - verts[0]);
    vec2f p_r1 = discard_Z(verts[0] - (vec3f)point);

    // vec3f x_values { r1_r2.x, r1_r3.x, p_r1.x };
    // vec3f y_values { r1_r2.y, r1_r3.y, p_r1.y };
    vec3f vecRes = crossProduct(vec3f { r1_r2.x, r1_r3.x, p_r1.x }, vec3f { r1_r2.y, r1_r3.y, p_r1.y });

    if (std::abs(vecRes.z) < 1)
        return vec3f { -1, 1, 1 };

    vecRes = vecRes / vecRes.z;
    return (vec3f { 1.0f - (vecRes.x + vecRes.y), vecRes.x, vecRes.y });
}