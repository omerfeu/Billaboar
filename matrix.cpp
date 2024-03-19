#include "matrix.hpp"

Matrix::Matrix(size_t rows, size_t columns) : rows(rows), columns(columns) {
    matrix = new float[rows*columns]();
}
Matrix::Matrix(const Matrix& m) : rows(m.rows), columns(m.columns) {
    matrix = new float[m.rows * m.columns];
    for (size_t i = 0; i < rows*columns; i++) {
        matrix[i] = m.matrix[i];
    }
}

float& Matrix::operator[](size_t index) {
    return this->matrix[index];
}
float Matrix::operator[](size_t index) const{
    return matrix[index];
}
float& Matrix::operator()(size_t row, size_t column){
    return this->matrix[row * columns + column];
}
float Matrix::operator()(size_t row, size_t column) const {
    return matrix[row * columns + column];
}

Matrix Matrix::operator+(const Matrix& rhs) const{
    Matrix result(rhs.rows, rhs.columns);
    for (size_t i = 0; i < rows*columns; i++) {
        result[i] = matrix[i] - rhs[i];
    }
    return result;
}

Matrix Matrix::operator-(const Matrix& rhs) const{
    Matrix result(rhs.rows, rhs.columns);
    for (size_t i = 0; i < rows*columns; i++) {
        result[i] = matrix[i] - rhs[i];
    }
    return result;
}

Matrix& Matrix::operator=(const Matrix& rhs) {
    if (this == &rhs) {
        return *this;
    }
    delete[] matrix;
    columns = rhs.columns;
    rows = rhs.rows;
    matrix = new float[rows * columns];
    for (size_t i = 0; i < rows*columns; i++) {
        matrix[i] = rhs.matrix[i];
    }
    return *this;
}


Matrix::~Matrix(){
    delete[] matrix;
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    for (size_t i = 0; i < matrix.rows; ++i) {
        for (size_t j = 0; j < matrix.columns; ++j) {
            os << matrix(i,j) << "  ";
        }
        os << std::endl;
    }
    return os;
}


