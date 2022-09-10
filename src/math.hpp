#ifndef MATH_H
#define MATH_H

#include <array>
#include <cmath>
#include <iostream>

template <typename T>
class vec3;

//////////////////////////////////////////////////////
// Vectores con 2 componentes
template <typename T>
class vec2 {
public:
    T x, y;
    vec2() : x(T()), y(T()) {}
    explicit vec2(const T l_x, const T l_y) : x(l_x), y(l_y) {}

    operator vec3<T>() const;

    template <typename U>
    operator vec2<U>();

    friend std::ostream& operator<<(std::ostream& out, const vec2& v2) {
        out << "v2(" << v2.x << ", " << v2.y << ")";
        return out;
    }

    void normalize() {
        float module = std::sqrt(dotProduct(static_cast<vec3<T>>(*this), static_cast<vec3<T>>(*this)));
        *this = *this / module;
    }
};

template <typename T>
template <typename U>
vec2<T>::operator vec2<U>() {
    return vec2<U> { (U)((*this).x), (U)((*this).y) };
}

// Operaciones basicas vec2

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
    vec3() : x(T()), y(T()), z(T()) {}
    explicit vec3(T l_x, T l_y, T l_z) : x(l_x), y(l_y), z(l_z) {}

    template <typename U>
    operator vec3<U>();

    friend std::ostream& operator<<(std::ostream& out, const vec3& v3) {
        out << "v3(" << v3.x << ", " << v3.y << ", " << v3.z << ")";
        return out;
    }

    void normalize() {
        float module = std::sqrt(dotProduct(*this, *this));
        *this = *this / module;
    }
};

template <typename T>
template <typename U>
vec3<T>::operator vec3<U>() {
    return vec3<U> { (U)((*this).x), (U)((*this).y), (U)((*this).z) };
}

// Operaciones basicas vec2

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
template <typename T>
vec2<T> discard_Z(vec3<T> v) {
    return vec2<T> { v.x, v.y };
}

////////////////////////////////////////////////////////////////
// Productos entre vectores

template <typename T>
const T dotProduct(const vec3<T>& v1, const vec3<T>& v2) {
    return T { v1.x * v2.x + v1.y * v2.y + v1.z * v2.z };
}

template <typename T>
const vec3<T> crossProduct(const vec3<T>& v1, const vec3<T>& v2) {
    return vec3<T> { v1.y * v2.z - v2.y * v1.z, v2.x * v1.z - v1.x * v2.z, v1.x * v2.y - v2.x * v1.y };
}

////////////////////////////////////////////////////////////////

using vec2i = vec2<int>;
using vec2f = vec2<float>;
using vec3i = vec3<int>;
using vec3f = vec3<float>;

////////////////////////////////////////////////////////////////
// Generic math vector

template <int size, typename T>
class vec {
public:
    std::array<T, size> data {};

    vec() = default;

    template <typename U>
    operator vec<size, U>();

    T& operator[](const int index) { return data[index]; }
    T operator[](const int index) const { return data[index]; }

    friend std::ostream& operator<<(std::ostream& out, const vec<size, T>& vector) {
        out << "v( ";
        for (int i = 0; i < size; i++) {
            out << vector.data[i] << ", ";
        }
        out << "\b\b )";
        return out;
    }
};

//////////////////////////////////////////////////////////////////////////////////
// Operaciones basicas

template <int size, typename T>
const vec<size, T> operator+(const vec<size, T>& lhs, const vec<size, T>& rhs) {
    vec<size, T> ret;
    for (int i = 0; i < size; i++) {
        ret[i] = lhs[i] + rhs[i];
    }
    return ret;
}

template <int size, typename T>
const vec<size, T> operator-(const vec<size, T>& lhs, const vec<size, T>& rhs) {
    vec<size, T> ret;
    for (int i = 0; i < size; i++) {
        ret[i] = lhs[i] - rhs[i];
    }
    return ret;
}

template <int size, typename T>
const vec<size, T> operator*(const vec<size, T>& lhs, const T rhs) {
    vec<size, T> ret;
    for (int i = 0; i < size; i++) {
        ret[i] = lhs[i] * rhs;
    }
    return ret;
}

template <int size, typename T>
const vec<size, T> operator/(const vec<size, T>& lhs, const T rhs) {
    vec<size, T> ret;
    for (int i = 0; i < size; i++) {
        ret[i] = lhs[i] / rhs;
    }
    return ret;
}

template <int size, typename T>
template <typename U>
vec<size, T>::operator vec<size, U>() {
    vec<size, U> ret;
    for (int i = 0; i < size; i++) {
        ret[i] = (U)(*this)[i];
    }
    return ret;
}

//////////////////////////////////////////////////////////////////////////////////
// PRODUCTO PUNTO

template <int size, typename T>
T dotProduct(const vec<size, T>& lhs, const vec<size, T>& rhs) {
    T ret = T();
    for (int i = 0; i < size; i++) {
        ret += lhs[i] * rhs[i];
    }
    return ret;
}

////////////////////////////////////////////////////////////////
// Geometric Baricentric Coords
vec3f toBarycentricCoord(vec3f* verts, vec2f point);
vec3f toBarycentricCoord(vec2f* verts, vec2f point);

#endif // MATH_H