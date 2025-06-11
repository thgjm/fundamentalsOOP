#pragma once
#include <vector>
#include <iostream>

using namespace std;

class Matrix {
private:
    vector<vector<double>> data;
    size_t rows;
    size_t cols;

public:
    Matrix(size_t rows, size_t cols);
    Matrix(const vector<vector<double>>& data);
    
    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }
    double& at(size_t i, size_t j) { return data[i][j]; }
    const double& at(size_t i, size_t j) const { return data[i][j]; }
    
    void randomize(double min, double max);
    void print() const;
    
    static bool canMultiply(const Matrix& a, const Matrix& b);
    
    friend ostream& operator<<(ostream& os, const Matrix& matrix);
};