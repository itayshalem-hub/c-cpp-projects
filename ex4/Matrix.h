// Matrix.h
#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <stdexcept>

typedef struct matrix_dims {
    int rows, cols;
} matrix_dims;

class Matrix {
private:
    int _rows;
    int _cols;
    float* _data;

    static constexpr float PRINT_THRESHOLD = 0.1f;
    static constexpr float RREF_EPSILON = 1e-5f;

public:
    /**
 * @brief Computes and returns the sum of all elements in the Matrix.
 */
    float sum() const;
    /**
     * @brief Default constructor.
     * Constructs a 1x1 matrix initialized to 0.
     */
    Matrix();

    /**
     * @brief Parameterized constructor. Constructs a rows x cols matrix initialized to 0.
     * @param rows Number of rows.
     * @param cols Number of columns.
     * @throws std::length_error If rows or cols are non-positive.
     */
    Matrix(int rows, int cols);

    /**
     * @brief Copy constructor.
     * Handles deep copying from another matrix.
     * @param other The matrix to copy from.
     */
    Matrix(const Matrix &other);

    /**
     * @brief Destructor.
     * Safely cleans up allocated dynamic memory.
     */
    ~Matrix();

    /**
     * @brief Copy assignment operator. Resizes the matrix if dimensions differ.
     * @param other The matrix to assign to this instance.
     * @return Reference to this matrix.
     */
    Matrix& operator=(const Matrix &other);

    /**
     * @brief Getter for the number of rows.
     */
    int get_rows() const;

    /**
     * @brief Getter for the number of columns.
     */
    int get_cols() const;

    /**
     * @brief Adds another matrix to this matrix element-wise.
     * @throws std::length_error If dimensions do not match.
     */
    Matrix& operator+=(const Matrix &other);

    /**
     * @brief Calculates element-wise addition of two matrices.
     * @throws std::length_error If dimensions do not match.
     */
    Matrix operator+(const Matrix &other) const;

    /**
     * @brief Performs matrix multiplication as defined in Linear Algebra.
     * @throws std::length_error If the columns of this do not match rows of other.
     */
    Matrix operator*(const Matrix &other) const;

    /**
     * @brief Performs scalar multiplication on the right.
     */
    Matrix operator*(float scalar) const;

    /**
     * @brief Performs scalar multiplication on the left.
     */
    friend Matrix operator*(float scalar, const Matrix &m);

    /**
     * @brief 2D parenthesis indexing operator for non-const matrix access.
     * @throws std::out_of_range If indices are out of bounds.
     */
    float& operator()(int i, int j);

    /**
     * @brief 2D parenthesis indexing operator for const matrix access.
     * @throws std::out_of_range If indices are out of bounds.
     */
    const float& operator()(int i, int j) const;

    /**
     * @brief 1D bracket indexing operator for non-const flattened access (row-wise).
     * @throws std::out_of_range If index is out of bounds.
     */
    float& operator[](int k);

    /**
     * @brief 1D bracket indexing operator for const flattened access (row-wise).
     * @throws std::out_of_range If index is out of bounds.
     */
    const float& operator[](int k) const;

    /**
     * @brief Output stream operator. Pretty prints the matrix using asterisks.
     */
    friend std::ostream& operator<<(std::ostream &os, const Matrix &m);

    /**
     * @brief Input stream operator. Fills matrix elements from a binary stream.
     */
    friend std::istream& operator>>(std::istream &is, Matrix &m);

    /**
     * @brief Prints all matrix elements as raw floats followed by a space and newlines.
     */
    void plain_print() const;

    /**
     * @brief Transforms the matrix in-place into its mathematical transpose.
     * @return Reference to this matrix.
     */
    Matrix& transpose();

    /**
     * @brief Flattens the matrix in-place into a column vector (row-wise).
     * @return Reference to this matrix.
     */
    Matrix& vectorize();

    /**
     * @brief Computes the Hadamard product (element-wise multiplication) with another matrix.
     * @throws std::length_error If dimensions do not match.
     */
    Matrix dot(const Matrix &other) const;

    /**
     * @brief Computes the Frobenius norm of the matrix.
     */
    float norm() const;

    /**
     * @brief Returns the index of the largest value in the matrix.
     * Ties return the lowest index.
     */
    int argmax() const;

    /**
     * @brief Computes and returns a new matrix in Reduced Row Echelon Form (RREF).
     */
    Matrix rref() const;
};

#endif //MATRIX_H