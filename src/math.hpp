#ifndef MATH_H
#define MATH_H

#include <iostream>

template <typename T>
class vec3;

//////////////////////////////////////////////////////
// Vectores con 2 componentes
template <typename T>
class vec2 {
public:
    T x, y;
    explicit vec2() : x(T()), y(T()) {}
    explicit vec2(const T l_x, const T l_y) : x(l_x), y(l_y) {}

    operator vec3<T>() const;

    friend std::ostream& operator<<(std::ostream& out, const vec2& v2) {
        out << "v2(" << v2.x << ", " << v2.y << ")";
        return out;
    }
};

template <typename T>
const vec2<T> operator+(const vec2<T>& v1, const vec2<T>& v2) {
    return vec2<T> { v1.x + v2.x, v1.y + v2.y };
}

template <typename T>
const vec2<T> operator-(const vec2<T>& v1, const vec2<T>& v2) {
    return vec2<T> { v1.x - v2.x, v1.y - v2.y };
}

template <typename T>
const vec2<T> operator*(const vec2<T>& v1, const T k) {
    return vec2<T> { v1.x * k, v1.y * k };
}

template <typename T>
const vec2<T> operator/(const vec2<T>& v1, const T k) {
    return vec2<T> { v1.x / k, v1.y / k };
}

//////////////////////////////////////////////////////
// Vectores con 3 componentes
template <typename T>
class vec3 {
public:
    T x, y, z;
    explicit vec3() : x(T()), y(T()), z(T()) {}
    explicit vec3(T l_x, T l_y, T l_z) : x(l_x), y(l_y), z(l_z) {}

    friend std::ostream& operator<<(std::ostream& out, const vec3& v3) {
        out << "v3(" << v3.x << ", " << v3.y << ", " << v3.z << ")";
        return out;
    }
};

template <typename T>
const vec3<T> operator+(const vec3<T>& v1, const vec3<T>& v2) {
    return vec3<T> { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

template <typename T>
const vec3<T> operator-(const vec3<T>& v1, const vec3<T>& v2) {
    return vec3<T> { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

template <typename T>
const vec3<T> operator*(const vec3<T>& v1, const T k) {
    return vec3<T> { v1.x * k, v1.y * k, v1.z * k };
}

template <typename T>
const vec3<T> operator/(const vec3<T>& v1, const T k) {
    return vec3<T> { v1.x / k, v1.y / k, v1.z / k };
}

// Typecast sobreescrito vec2<T> a vec3<T>
template <typename T>
vec2<T>::operator vec3<T>() const {
    return vec3<T> { x, y, T() };
}

////////////////////////////////////////////////////////////////
// Productos entre vectores

template <typename T>
const T dotProduct(const vec3<T>& v1, const vec3<T>& v2) {
    return T { v1.x * v2.x + v1.y * v2.y + v1.z * v2.z };
}

template <typename T>
const vec3<T> crossProduct(const vec3<T>& v1, const vec3<T>& v2) {
    return vec3<T> { v1.y * v2.z - v2.y * v1.z, v2.x * v1.z - v1.x * v2.z,
                     v1.x * v2.y - v2.x * v1.y };
}

using vec2i = vec2<int>;
using vec2f = vec2<float>;
using vec3i = vec3<int>;
using vec3f = vec3<float>;

#endif // MATH_H