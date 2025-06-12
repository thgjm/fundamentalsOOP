#include "../headers/SequentialMultiplier.h"
#include "../headers/ParallelMultiplier.h"
#include <chrono>
#include <iomanip>
#include <memory>
#include <thread>
#include <limits>

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

//helper function to get valid input
template<typename T>
T getInput(const string& prompt, T min_val, T max_val) {
    T value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= min_val && value <= max_val) {
            break;
        }
        cout << "Invalid input. Please try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return value;
}

int main() {
    //get available CPU threads
    size_t max_threads = thread::hardware_concurrency();
    cout << "Your CPU has " << max_threads << " available threads.\n\n";

    //get matrix size from user
    size_t size = getInput<size_t>(
        "Enter matrix size (1-10000): ", 
        1, 
        10000
    );

    //get number of threads from user
    size_t numThreads = getInput<size_t>(
        "Enter number of threads (1-" + to_string(max_threads) + "): ",
        1,
        max_threads
    );

    cout << "\nConfiguration:\n"
         << "- Matrix size: " << size << "x" << size << "\n"
         << "- Number of threads: " << numThreads << "\n"
         << "- Available CPU threads: " << max_threads << "\n\n";

    //test matrices
    Matrix a(size, size);
    Matrix b(size, size);
    
    cout << "Initializing matrices...\n";
    a.randomize();
    b.randomize();
    
    //multipliers
    SequentialMultiplier seqMult;
    ParallelMultiplier parMult(numThreads);
    
    cout << "\nRunning sequential multiplication...\n";
    runTest(a, b, seqMult);
    
    cout << "\nRunning parallel multiplication...\n";
    runTest(a, b, parMult);
    
    return 0;
} 