#include "..headers/Matrix.h"
#include <random>
#include <iomanip>

Matrix::Matrix(size_t rows, size_t cols) 
    : rows(rows), cols(cols), data(rows, vector<double>(cols, 0.0)) {}

Matrix::Matrix(const vector<vector<double>>& data)
    : data(data), rows(data.size()), cols(data[0].size()) {}

void Matrix::randomize(double min, double max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(min, max);

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            data[i][j] = dis(gen);
        }
    }
}

void Matrix::print() const {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            cout << fixed << setprecision(2) << data[i][j] << " ";
        }
        cout << "\n";
    }
}

bool Matrix::canMultiply(const Matrix& a, const Matrix& b) {
    return a.getCols() == b.getRows();
}

ostream& operator<<(ostream& os, const Matrix& matrix) {
    for (size_t i = 0; i < matrix.rows; ++i) {
        for (size_t j = 0; j < matrix.cols; ++j) {
            os << fixed << setprecision(2) << matrix.data[i][j] << " ";
        }
        os << "\n";
    }
    return os;
} 