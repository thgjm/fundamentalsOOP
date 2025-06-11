#pragma once
#include "MatrixMultiplier.h"

class SequentialMultiplier : public MatrixMultiplier {
public:
    Matrix multiply(const Matrix& a, const Matrix& b) override;
    const char* getName() const override { return "Sequential"; }
}; 