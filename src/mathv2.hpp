#ifndef MATH_H
#define MATH_H

#include <array>
#include <cmath>
#include <iostream>

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

//////////////////////////////////////////////////////////////////////////////////
// Objetos especificos

#endif // MATH_H