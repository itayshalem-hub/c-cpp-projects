// Activation.h
#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"

namespace activation {
    /**
     * @brief Applies element-wise Rectified Linear Unit activation.
     */
    Matrix relu(const Matrix &m);

    /**
     * @brief Applies Softmax activation function over all elements of the matrix as a distribution.
     */
    Matrix softmax(const Matrix &m);
}

#endif //ACTIVATION_H