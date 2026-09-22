// Activation.cpp
#include "Activation.h"
#include <cmath>

Matrix activation::relu(const Matrix &m) {
    Matrix result(m);
    int total_elements = m.get_rows() * m.get_cols();
    for (int i = 0; i < total_elements; ++i) {
        if (result[i] < 0.0f) {
            result[i] = 0.0f;
        }
    }
    return result;
}

Matrix activation::softmax(const Matrix &m) {
    Matrix result(m);
    int total_elements = m.get_rows() * m.get_cols();
    float sum = 0.0f;
    for (int i = 0; i < total_elements; ++i) {
        result[i] = std::exp(m[i]);
        sum += result[i];
    }

    for (int i = 0; i < total_elements; ++i) {
        result[i] /= sum;
    }
    return result;
}