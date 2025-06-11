#include "headers/SequentialMultiplier.h"
#include <stdexcept>

Matrix SequentialMultiplier::multiply(const Matrix& a, const Matrix& b) {
    if (!Matrix::canMultiply(a, b)) {
        throw invalid_argument("Matrix dimensions are not compatible for multiplication");
    }

    Matrix result(a.getRows(), b.getCols());
    
    for (size_t i = 0; i < a.getRows(); ++i) {
        for (size_t j = 0; j < b.getCols(); ++j) {
            double sum = 0.0;
            for (size_t k = 0; k < a.getCols(); ++k) {
                sum += a.at(i, k) * b.at(k, j);
            }
            result.at(i, j) = sum;
        }
    }
    
    return result;
}