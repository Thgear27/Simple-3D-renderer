#ifndef MATRIZ_H
#define MATRIZ_H

#include <vector>
#include <iostream>
#include "math.hpp"

class Matrix {
private:
    int num_rows;    
    int num_columns;    

    std::vector<std::vector<float>> raw;
public:
    Matrix(int rows = 4, int columns = 4);
    Matrix(const Matrix& cpymat);
    ~Matrix();

    static Matrix Identity(int size);

    inline std::vector<float>&  operator[] (int index) { return raw[index]; }
    inline const std::vector<float>&  operator[] (int index) const { return raw[index]; }
    friend std::ostream& operator<< (std::ostream& out, const Matrix& mat);
    friend const Matrix  operator*  (const Matrix& lhs, const Matrix& rhs);
    friend const Matrix  operator+  (const Matrix& lhs, const Matrix& rhs);
    friend const Matrix  operator-  (const Matrix& lhs, const Matrix& rhs);
    // vec2f = mat2x3 * vec3f;
    friend vec2f mult2x3 (const Matrix& lhs, const vec3f& rhs);
    // vec3f = mat3x3 * vec3f;
    friend vec3f mult3x3 (const Matrix& lhs, const vec3f& rhs);

    void setCol(int col, const vec3f& vec);
    void setRow(int row, const vec3f& vec);
    void setCol(int col, const vec2f& vec);
    void setRow(int row, const vec2f& vec);


    void inverse();
    void addToRow(int rowTo, int rowFrom, float mult);
    void multiplyRowBy(int row, float multNumber);

    Matrix& operator= (const Matrix& cpymat);

    int getNumRows()    const { return num_rows; }
    int getNumColumns() const { return num_columns; }
};

#endif // MATRIZ_H 
