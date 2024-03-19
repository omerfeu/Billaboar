#include <iostream>

class Matrix {
private:
    float* matrix;
public:
    size_t rows;
    size_t columns;
    Matrix(size_t rows, size_t columns);
    Matrix(const Matrix& m);
    float& operator[](size_t index);
    float operator[](size_t index) const;
    float& operator()(size_t row, size_t column);
    float operator()(size_t row, size_t column) const;
    Matrix operator+(const Matrix& rhs) const;
    Matrix operator-(const Matrix& rhs) const;
    Matrix& operator=(const Matrix& rhs) ;
    ~Matrix();
    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
};
