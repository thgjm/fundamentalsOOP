//multiplier interface
//strategy pattern used

#pragma once
#include "Matrix.h"
#include <stdexcept>

class MatrixMultiplier {
protected:
    void validateMatrices(const Matrix& a, const Matrix& b) const {
        if (a.getRows() == 0 || a.getCols() == 0 || b.getRows() == 0 || b.getCols() == 0) {
            throw std::invalid_argument("Cannot multiply empty matrices");
        }
        if (!Matrix::canMultiply(a, b)) {
            throw std::invalid_argument("Matrix dimensions are not compatible for multiplication");
        }
    }

public:
    virtual Matrix multiply(const Matrix& a, const Matrix& b) = 0;
    virtual const char* getName() const = 0;
    virtual ~MatrixMultiplier() = default;
}; 