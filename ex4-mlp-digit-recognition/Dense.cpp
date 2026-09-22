// Dense.cpp
#include "Dense.h"

Dense::Dense() : _weights(Matrix()), _bias(Matrix()), _act_func(nullptr) {}

Dense::Dense(const Matrix &weights, const Matrix &bias, ActFunc act_func)
    : _weights(weights), _bias(bias), _act_func(act_func) {}

Matrix Dense::get_weights() const {
    return _weights;
}

Matrix Dense::get_bias() const {
    return _bias;
}

ActFunc Dense::get_activation() const {
    return _act_func;
}

Matrix Dense::operator()(const Matrix &m) const {
    return _act_func((_weights * m) + _bias);
}