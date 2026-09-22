// Dense.h
#ifndef DENSE_H
#define DENSE_H

#include "Matrix.h"

typedef Matrix (*ActFunc)(const Matrix &);

class Dense {
private:
    Matrix _weights;
    Matrix _bias;
    ActFunc _act_func;

public:
    /**
     * @brief Default constructor.
     * Sets components to default and activation to nullptr.
     */
    Dense();

    /**
     * @brief Parameterized constructor. Initialized a layer with given weights, bias, and activation.
     */
    Dense(const Matrix &weights, const Matrix &bias, ActFunc act_func);

    /**
     * @brief Getter for weights matrix.
     */
    Matrix get_weights() const;

    /**
     * @brief Getter for bias matrix.
     */
    Matrix get_bias() const;

    /**
     * @brief Getter for the activation function pointer.
     */
    ActFunc get_activation() const;

    /**
     * @brief Functor execution operator. Applies the layer formulas to an input matrix.
     */
    Matrix operator()(const Matrix &m) const;
};
#endif //DENSE_H