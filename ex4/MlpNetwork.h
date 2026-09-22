// MlpNetwork.h
#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Dense.h"

#define MLP_SIZE 4

const matrix_dims img_dims = {28, 28};
const matrix_dims weights_dims[] = {{128, 784},
									{64,  128},
									{20,  64},
									{10,  20}};
const matrix_dims bias_dims[] = {{128, 1},
								 {64,  1},
								 {20,  1},
								 {10,  1}};

typedef struct digit {
	unsigned int value;
	float probability;
} digit;

class MlpNetwork {
private:
	Dense _layers[MLP_SIZE];

public:
	/**
	 * @brief Parameterized constructor. Constructs the network utilizing individual layer buffers.
	 */
	MlpNetwork(const Matrix weights[], const Matrix biases[], const ActFunc acts[]);

	/**
	 * @brief Execution functor tracking input values consecutively across all 4 dense layers.
	 */
	digit operator()(const Matrix &input) const;
};

#endif // MLPNETWORK_H