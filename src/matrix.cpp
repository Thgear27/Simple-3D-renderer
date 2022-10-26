#include "matrix.hpp"

Matrix::~Matrix() {}

Matrix::Matrix(int rows, int columns) :  num_rows(rows), num_columns(columns) {
    raw.resize(num_rows);

    for (int i = 0; i < num_rows; i++) {
        raw[i].resize(num_columns);
    }

    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_columns; j++) raw[i][j] = 0;
    }
}

Matrix::Matrix(const Matrix& cpymat) {
    num_rows    = cpymat.num_rows;
    num_columns = cpymat.num_columns;

    raw.resize(num_rows);

    for (int i = 0; i < num_rows; i++) {
        raw[i].resize(num_columns);
    }
    
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_columns; j++) {
            raw[i][j] = cpymat.raw[i][j];
        }
    }
}


Matrix Matrix::Identity(int size) {
    Matrix mat { size, size };
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            mat[i][j] = (i == j) ? 1 : 0 ;
        }
    }
    return mat;
}

std::ostream& operator<< (std::ostream& out, const Matrix& mat) {
    for (int i = 0; i < mat.num_rows; i++) {
        out << "[ ";
        for (int j = 0; j < mat.num_columns; j++)
            out << mat.raw[i][j] << " ";
        out << "]\n";
    }
    return out;
}

Matrix& Matrix::operator= (const Matrix& cpymat) {
    if (this == &cpymat) 
        return *this;
    
    num_rows    = cpymat.num_rows;
    num_columns = cpymat.num_columns;
    raw.resize(num_rows);

    for (int i = 0; i < num_rows; i++) {
        raw[i].resize(num_columns);
    }
    
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_columns; j++) {
            raw[i][j] = cpymat.raw[i][j];
        }
    }
    
    return *this;
}
    
const Matrix operator* (const Matrix& lhs, const Matrix& rhs) {
    if (lhs.num_columns != rhs.num_rows)
        std::cerr << "Error, no se puede multiplicar esta matriz" << '\n';

    Matrix result { lhs.num_rows, rhs.num_columns };

    for (int i = 0; i < lhs.num_rows; i++) {
        for (int j = 0; j < rhs.num_columns; j++) {
            for (int item_index = 0; item_index < lhs.num_columns; item_index++) {
                result[i][j] += lhs.raw[i][item_index] * rhs.raw[item_index][j];
            }
        }
    }
    return result; 
}

const Matrix operator+ (const Matrix& lhs, const Matrix& rhs) {
    if (lhs.num_rows != rhs.num_rows || lhs.num_columns != rhs.num_columns) 
        std::cerr << "Error, no se puede sumar esta matriz" << '\n';

    Matrix result { lhs.num_rows, lhs.num_columns };
    
    for (int i = 0; i < lhs.num_rows; i++) {
        for (int j = 0; j < lhs.num_columns; j++)
            result[i][j] = lhs.raw[i][j] + rhs.raw[i][j];
    }
    return result; 
}

const Matrix operator- (const Matrix& lhs, const Matrix& rhs) {
    if (lhs.num_rows != rhs.num_rows || lhs.num_columns != rhs.num_columns) 
        std::cerr << "Error, no se puede sumar esta matriz" << '\n';

    Matrix result { lhs.num_rows, lhs.num_columns };
    
    for (int i = 0; i < lhs.num_rows; i++) {
        for (int j = 0; j < lhs.num_columns; j++)
            result[i][j] = lhs.raw[i][j] - rhs.raw[i][j];
    }
    return result; 
}

    // vec2f = mat2x3 * vec3f;
vec2f mult2x3 (const Matrix& lhs, const vec3f& rhs) {
    if (lhs.num_columns != 3) return vec2f{ 1, 1 };
    vec2f ret { 0, 0 };
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            ret[i] += lhs[i][j] * rhs[j];
        }
    }
    return ret;
}
    // vec3f = mat3x3 * vec3f;
vec3f mult3x3 (const Matrix& lhs, const vec3f& rhs) {
    if (lhs.num_columns != 3) {
        return vec3f{ 1, 1, 1 };
    }
    vec3f ret { 0, 0, 0 };
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            ret[i] += lhs[i][j] * rhs[j];
        }
    }
    return ret;
}

void Matrix::setCol(int col, const vec3f& vec) {
    if (num_rows < 3) {
        std::cerr << "No se puede papu setCol(col, vec3f)\n";
        return;
    } 
    for (int i = 0; i < 3; i++)
        raw[i][col] = vec[i];
}

void Matrix::setRow(int row, const vec3f& vec) {
    if (num_columns < 3) {
        std::cerr << "No se puede papu setRow(row, vec3f)\n";
        return;
    } 
    for (int i = 0; i < 3; i++)
        raw[row][i] = vec[i];
}

void Matrix::setCol(int col, const vec2f& vec) {
    if (num_rows < 2) {
        std::cerr << "No se puede papu setCol(col, vec2f)\n";
        return;
    } 
    for (int i = 0; i < 2; i++)
        raw[i][col] = vec[i];
}

void Matrix::setRow(int row, const vec2f& vec) {
    if (num_columns < 2) {
        std::cerr << "No se puede papu setRow(row, vec2f)\n";
        return;
    } 
    for (int i = 0; i < 2; i++)
        raw[row][i] = vec[i];
}

void Matrix::addToRow(int rowTo, int rowFrom, float mult) {
    for (int i = 0; i < num_columns; i++) {
        raw[rowTo][i] += raw[rowFrom][i] * mult;
    }
}
void Matrix::multiplyRowBy(int row, float multNumber) {
    for (int i = 0; i < num_columns; i++)
        raw[row][i] *= multNumber;
}

void Matrix::inverse() {
    if (num_columns != num_rows) {
        std::cout << "No se puede invertir la matriz con \n"
                  << "Numero de columnas:" << num_columns << '\n'
                  << "Numero de filas:   " << num_rows << '\n';
    }
    Matrix result = Matrix::Identity(num_columns);
    
    for (int col = 0; col < num_columns; col++) {
        float pivot = raw[col][col];
        if (pivot == 0) {
            return;
        }

        multiplyRowBy(col, 1.0f / pivot);
        result.multiplyRowBy(col, 1.0f / pivot);

        for (int row = 0; row < num_rows; row++) {
            if (col == row) continue;
            float current = raw[row][col];
            addToRow(row, col, -current);
            result.addToRow(row, col, -current);
        }
    }
    *this = result;
}