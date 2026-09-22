// MlpNetwork.cpp
#include "MlpNetwork.h"

MlpNetwork::MlpNetwork(const Matrix weights[], const Matrix biases[], const ActFunc acts[]) {
    for (int i = 0; i < MLP_SIZE; ++i) {
        _layers[i] = Dense(weights[i], biases[i], acts[i]);
    }
}

digit MlpNetwork::operator()(const Matrix &input) const {
    Matrix current = input;
    for (int i = 0; i < MLP_SIZE; ++i) {
        current = _layers[i](current);
    }

    int chosen_idx = current.argmax();
    digit result;
    result.value = static_cast<unsigned int>(chosen_idx);
    result.probability = current[chosen_idx];
    return result;
}