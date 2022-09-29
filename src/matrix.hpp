#ifndef MATRIZ_H
#define MATRIZ_H

#include <vector>
#include <iostream>

class Matrix {
private:
    int num_rows;    
    int num_columns;    

    std::vector<std::vector<float>> raw;
public:
    Matrix(int rows = 4, int columns = 4);
    Matrix(const Matrix& cpymat);
    ~Matrix();

    static const Matrix& Identity(int size);

    std::vector<float>&  operator[] (int index); 
    const std::vector<float>&  operator[] (int index) const; 
    friend std::ostream& operator<< (std::ostream& out, const Matrix& mat);
    friend const Matrix  operator*  (const Matrix& lhs, const Matrix& rhs);
    friend const Matrix  operator+  (const Matrix& lhs, const Matrix& rhs);
    friend const Matrix  operator-  (const Matrix& lhs, const Matrix& rhs);

    Matrix& operator= (const Matrix& cpymat);

    int getNum_rows()    const { return num_rows; }
    int getNum_columns() const { return num_columns; }
};

#endif // MATRIZ_H 
