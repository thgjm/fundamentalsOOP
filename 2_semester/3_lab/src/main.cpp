#include "../headers/SequentialMultiplier.h"
#include "../headers/ParallelMultiplier.h"
#include <chrono>
#include <iomanip>
#include <memory>

const size_t MAX_PRINT = 15; //only print matrices if they're smaller than this

void printMatrixOperation(const Matrix& a, const Matrix& b, const Matrix& result) {
    cout << "\nMatrix A:\n" << a;
    cout << "\nMatrix B:\n" << b;
    cout << "\nResult Matrix (A * B):\n" << result;
    cout << "\n";
}

void runTest(const Matrix& a, const Matrix& b, MatrixMultiplier& multiplier) {
    auto start = chrono::high_resolution_clock::now();
    
    Matrix result = multiplier.multiply(a, b);
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    
    cout << multiplier.getName() << " Multiplication took: " 
              << duration.count() << " ms\n";
    
    //print matrices if they're not too big
    if (a.getRows() <= MAX_PRINT && a.getCols() <= MAX_PRINT &&
        b.getRows() <= MAX_PRINT && b.getCols() <= MAX_PRINT) {
        printMatrixOperation(a, b, result);
    }
}

int main() {

    //test matrices
    const size_t size = 1000; //can be changed to whatever size
    Matrix a(size, size);
    Matrix b(size, size);
    
    cout << "Initializing matrices of size " << size << "x" << size << "...\n";
    a.randomize();
    b.randomize();
    
    //multipliers
    SequentialMultiplier seqMult;
    ParallelMultiplier parMult;
    
    cout << "\nRunning sequential multiplication...\n";
    runTest(a, b, seqMult);
    
    cout << "\nRunning parallel multiplication...\n";
    runTest(a, b, parMult);
    
    return 0;
} 