// Matrix.cpp
#include "Matrix.h"
#include <cmath>

Matrix::Matrix() : _rows(1), _cols(1), _data(new float[1]) {
    _data[0] = 0.0f;
}

Matrix::Matrix(int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        throw std::length_error("Matrix dimensions must be strictly positive.");
    }
    _rows = rows;
    _cols = cols;
    _data = new float[_rows * _cols];
    for (int i = 0; i < _rows * _cols; ++i) {
        _data[i] = 0.0f;
    }
}

Matrix::Matrix(const Matrix &other) : _rows(other._rows), _cols(other._cols), _data(new float[other._rows * other._cols]) {
    for (int i = 0; i < _rows * _cols; ++i) {
        _data[i] = other._data[i];
    }
}

Matrix::~Matrix() {
    delete[] _data;
}

Matrix& Matrix::operator=(const Matrix &other) {
    if (this == &other) {
        return *this;
    }

    if (_rows * _cols != other._rows * other._cols) {
        float* new_data = new float[other._rows * other._cols];
        delete[] _data;
        _data = new_data;
    }

    _rows = other._rows;
    _cols = other._cols;
    for (int i = 0; i < _rows * _cols; ++i) {
        _data[i] = other._data[i];
    }
    return *this;
}

int Matrix::get_rows() const {
    return _rows;
}

int Matrix::get_cols() const {
    return _cols;
}

Matrix& Matrix::operator+=(const Matrix &other) {
    if (_rows != other._rows || _cols != other._cols) {
        throw std::length_error("Matrix dimensions must match for addition.");
    }
    for (int i = 0; i < _rows * _cols; ++i) {
        _data[i] += other._data[i];
    }
    return *this;
}

Matrix Matrix::operator+(const Matrix &other) const {
    if (_rows != other._rows || _cols != other._cols) {
        throw std::length_error("Matrix dimensions must match for addition.");
    }
    Matrix result(*this);
    result += other;
    return result;
}

Matrix Matrix::operator*(const Matrix &other) const {
    if (_cols != other._rows) {
        throw std::length_error("Matrix inner dimensions must match for multiplication.");
    }
    Matrix result(_rows, other._cols);
    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < other._cols; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < _cols; ++k) {
                sum += _data[i * _cols + k] * other._data[k * other._cols + j];
            }
            result._data[i * result._cols + j] = sum;
        }
    }
    return result;
}

Matrix Matrix::operator*(float scalar) const {
    Matrix result(_rows, _cols);
    for (int i = 0; i < _rows * _cols; ++i) {
        result._data[i] = _data[i] * scalar;
    }
    return result;
}

Matrix operator*(float scalar, const Matrix &m) {
    return m * scalar;
}

float& Matrix::operator()(int i, int j) {
    if (i < 0 || i >= _rows || j < 0 || j >= _cols) {
        throw std::out_of_range("Matrix index out of bounds.");
    }
    return _data[i * _cols + j];
}

const float& Matrix::operator()(int i, int j) const {
    if (i < 0 || i >= _rows || j < 0 || j >= _cols) {
        throw std::out_of_range("Matrix index out of bounds.");
    }
    return _data[i * _cols + j];
}

float& Matrix::operator[](int k) {
    if (k < 0 || k >= _rows * _cols) {
        throw std::out_of_range("Flattened matrix index out of bounds.");
    }
    return _data[k];
}

const float& Matrix::operator[](int k) const {
    if (k < 0 || k >= _rows * _cols) {
        throw std::out_of_range("Flattened matrix index out of bounds.");
    }
    return _data[k];
}

std::ostream& operator<<(std::ostream &os, const Matrix &m) {
    for (int i = 0; i < m._rows; ++i) {
        for (int j = 0; j < m._cols; ++j) {
            if (m._data[i * m._cols + j] > Matrix::PRINT_THRESHOLD) {
                os << "**";
            } else {
                os << "  ";
            }
        }
        os << std::endl;
    }
    return os;
}

std::istream& operator>>(std::istream &is, Matrix &m) {
    is.read(reinterpret_cast<char*>(m._data), m._rows * m._cols * sizeof(float));
    return is;
}

void Matrix::plain_print() const {
    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < _cols; ++j) {
            std::cout << _data[i * _cols + j] << " ";
        }
        std::cout << std::endl;
    }
}

Matrix& Matrix::transpose() {
    float* temp = new float[_rows * _cols];
    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < _cols; ++j) {
            temp[j * _rows + i] = _data[i * _cols + j];
        }
    }
    delete[] _data;
    _data = temp;
    int swap_val = _rows;
    _rows = _cols;
    _cols = swap_val;
    return *this;
}

Matrix& Matrix::vectorize() {
    _rows = _rows * _cols;
    _cols = 1;
    return *this;
}

Matrix Matrix::dot(const Matrix &other) const {
    if (_rows != other._rows || _cols != other._cols) {
        throw std::length_error("Matrix dimensions must match for Hadamard dot product.");
    }
    Matrix result(_rows, _cols);
    for (int i = 0; i < _rows * _cols; ++i) {
        result._data[i] = _data[i] * other._data[i];
    }
    return result;
}

float Matrix::norm() const {
    float sum_sq = 0.0f;
    for (int i = 0; i < _rows * _cols; ++i) {
        sum_sq += _data[i] * _data[i];
    }
    return std::sqrt(sum_sq);
}

int Matrix::argmax() const {
    int max_idx = 0;
    float max_val = _data[0];
    for (int i = 1; i < _rows * _cols; ++i) {
        if (_data[i] > max_val) {
            max_val = _data[i];
            max_idx = i;
        }
    }
    return max_idx;
}

Matrix Matrix::rref() const {
    Matrix result(*this);
    int lead = 0;
    int rCount = result._rows;
    int cCount = result._cols;

    for (int r = 0; r < rCount; ++r) {
        if (lead >= cCount) {
            break;
        }
        int i = r;
        while (std::abs(result._data[i * cCount + lead]) < RREF_EPSILON) {
            i++;
            if (i == rCount) {
                i = r;
                lead++;
                if (lead == cCount) {
                    return result;
                }
            }
        }

        if (i != r) {
            for (int c = 0; c < cCount; ++c) {
                float temp = result._data[r * cCount + c];
                result._data[r * cCount + c] = result._data[i * cCount + c];
                result._data[i * cCount + c] = temp;
            }
        }

        float pivot = result._data[r * cCount + lead];
        if (std::abs(pivot) > RREF_EPSILON) {
            for (int c = 0; c < cCount; ++c) {
                result._data[r * cCount + c] /= pivot;
            }
        }

        for (int j = 0; j < rCount; ++j) {
            if (j != r) {
                float factor = result._data[j * cCount + lead];
                for (int c = 0; c < cCount; ++c) {
                    result._data[j * cCount + c] -= factor * result._data[r * cCount + c];
                }
            }
        }
        lead++;
    }
    return result;
}