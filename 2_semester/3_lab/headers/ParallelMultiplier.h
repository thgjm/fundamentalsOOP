#pragma once
#include "MatrixMultiplier.h"
#include <thread>
#include <stdexcept>

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

ParallelMultiplier::ParallelMultiplier(size_t numThreads)
    : numThreads(numThreads) {}

void ParallelMultiplier::multiplyRange(const Matrix& a, const Matrix& b, 
                                     Matrix& result, size_t startRow, size_t endRow) {
    for (size_t i = startRow; i < endRow; ++i) {
        for (size_t j = 0; j < b.getCols(); ++j) {
            double sum = 0.0;
            for (size_t k = 0; k < a.getCols(); ++k) {
                sum += a.at(i, k) * b.at(k, j);
            }
            result.at(i, j) = sum;
        }
    }
}

Matrix ParallelMultiplier::multiply(const Matrix& a, const Matrix& b) {
    if (!Matrix::canMultiply(a, b)) {
        throw invalid_argument("Matrix dimensions are not compatible for multiplication");
    }

    Matrix result(a.getRows(), b.getCols());
    vector<thread> threads;
    
    // Calculate rows per thread
    size_t rowsPerThread = a.getRows() / numThreads;
    size_t remainingRows = a.getRows() % numThreads;
    
    size_t startRow = 0;
    
    // Create and launch threads
    for (size_t i = 0; i < numThreads; ++i) {
        size_t threadRows = rowsPerThread + (i < remainingRows ? 1 : 0);
        size_t endRow = startRow + threadRows;
        
        threads.emplace_back(&ParallelMultiplier::multiplyRange, this,
                           ref(a), ref(b), ref(result),
                           startRow, endRow);
        
        startRow = endRow;
    }
    
    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
    
    return result;
} 