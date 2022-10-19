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


const Matrix& Matrix::Identity(int size) {
    static Matrix mat { size, size };
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
        std::cerr << "Error, no se puede multiplicar esta matris" << '\n';

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
        std::cerr << "Error, no se puede sumar esta matris" << '\n';

    Matrix result { lhs.num_rows, lhs.num_columns };
    
    for (int i = 0; i < lhs.num_rows; i++) {
        for (int j = 0; j < lhs.num_columns; j++)
            result[i][j] = lhs.raw[i][j] + rhs.raw[i][j];
    }
    return result; 
}

const Matrix operator- (const Matrix& lhs, const Matrix& rhs) {
    if (lhs.num_rows != rhs.num_rows || lhs.num_columns != rhs.num_columns) 
        std::cerr << "Error, no se puede sumar esta matris" << '\n';

    Matrix result { lhs.num_rows, lhs.num_columns };
    
    for (int i = 0; i < lhs.num_rows; i++) {
        for (int j = 0; j < lhs.num_columns; j++)
            result[i][j] = lhs.raw[i][j] - rhs.raw[i][j];
    }
    return result; 
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
        if (pivot == 0) return;

        multiplyRowBy(col, 1.0f / pivot);
        result.multiplyRowBy(col, 1.0f / pivot);

        for (int row = 0; row < num_rows; row++) {
            if (col == row) continue;
            float current = raw[row][col];
            addToRow(row, col, -current);
            result.addToRow(row, col, -current);
        }
    }
}