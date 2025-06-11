#pragma once
#include "MatrixMultiplier.h"
#include <thread>

class ParallelMultiplier : public MatrixMultiplier {
private:
    size_t numThreads;
    void multiplyRange(const Matrix& a, const Matrix& b, Matrix& result, 
                      size_t startRow, size_t endRow);

public:
    explicit ParallelMultiplier(size_t numThreads = thread::hardware_concurrency());
    Matrix multiply(const Matrix& a, const Matrix& b) override;
    const char* getName() const override { return "Parallel"; }
};