//multiplier interface
//strategy pattern used

#pragma once
#include "Matrix.h"

class MatrixMultiplier {
public:
    virtual ~MatrixMultiplier() = default;
    virtual Matrix multiply(const Matrix& a, const Matrix& b) = 0;
    virtual const char* getName() const = 0;
}; 