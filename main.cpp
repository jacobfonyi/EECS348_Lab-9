//EECS 348 Lab 9 Main File
//C++ programming with matrices
//Input: matricies
//Outputs: modified matrix
//Collaboratoras: ChatGPT
//Author: Jacob Fonyi
//Date: 4/8/25

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <typeinfo>
#include <stdexcept>

template <typename T>
class Matrix {
private:
    size_t size;
    std::vector<std::vector<T>> data;

public:
    // Constructor
    Matrix(size_t n) : size(n), data(n, std::vector<T>(n)) {}

    // Getter for size
    size_t getSize() const { return size; }

    // Access operator
    std::vector<T>& operator[](size_t index) { return data[index]; }
    const std::vector<T>& operator[](size_t index) const { return data[index]; }

    // Matrix addition
    Matrix<T> operator+(const Matrix<T>& other) const {
        if (size != other.size) {
            throw std::invalid_argument("Matrix sizes don't match for addition");
        }
        Matrix<T> result(size);
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                result[i][j] = data[i][j] + other[i][j];
            }
        }
        return result;
    }

    // Matrix multiplication
    Matrix<T> operator*(const Matrix<T>& other) const {
        if (size != other.size) {
            throw std::invalid_argument("Matrix sizes don't match for multiplication");
        }
        Matrix<T> result(size);
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                for (size_t k = 0; k < size; ++k) {
                    result[i][j] += data[i][k] * other[k][j];
                }
            }
        }
        return result;
    }

    // Display matrix
    void display() const {
        for (const auto& row : data) {
            for (const auto& val : row) {
                std::cout << std::setw(10) << val;
            }
            std::cout << '\n';
        }
    }

    // Sum of main diagonal
    T sumMainDiagonal() const {
        T sum = 0;
        for (size_t i = 0; i < size; ++i) {
            sum += data[i][i];
        }
        return sum;
    }

    // Sum of secondary diagonal
    T sumSecondaryDiagonal() const {
        T sum = 0;
        for (size_t i = 0; i < size; ++i) {
            sum += data[i][size - 1 - i];
        }
        return sum;
    }

    // Swap rows
    void swapRows(size_t row1, size_t row2) {
        if (row1 >= size || row2 >= size) {
            throw std::out_of_range("Row index out of bounds");
        }
        std::swap(data[row1], data[row2]);
    }

    // Swap columns
    void swapColumns(size_t col1, size_t col2) {
        if (col1 >= size || col2 >= size) {
            throw std::out_of_range("Column index out of bounds");
        }
        for (size_t i = 0; i < size; ++i) {
            std::swap(data[i][col1], data[i][col2]);
        }
    }

    // Update element
    void updateElement(size_t row, size_t col, T value) {
        if (row >= size || col >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        data[row][col] = value;
    }
};

// Function to read matrix from file
template <typename T>
void readMatricesFromFile(const std::string& filename, Matrix<T>& mat1, Matrix<T>& mat2) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    size_t n;
    int type_flag;
    file >> n >> type_flag;

    mat1 = Matrix<T>(n);
    mat2 = Matrix<T>(n);

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            file >> mat1[i][j];
        }
    }

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            file >> mat2[i][j];
        }
    }

    file.close();
}

// Polymorphic function to swap rows (works with vector<int> or vector<double>)
template <typename Container>
void polymorphicSwapRows(Container& data, size_t row1, size_t row2, size_t size) {
    if (row1 >= size || row2 >= size) {
        throw std::out_of_range("Row index out of bounds");
    }
    std::swap(data[row1], data[row2]);
}

// Polymorphic function to swap columns (works with vector<int> or vector<double>)
template <typename Container>
void polymorphicSwapColumns(Container& data, size_t col1, size_t col2, size_t size) {
    if (col1 >= size || col2 >= size) {
        throw std::out_of_range("Column index out of bounds");
    }
    for (size_t i = 0; i < size; ++i) {
        std::swap(data[i][col1], data[i][col2]);
    }
}

// Template function to update element (works with int or double)
template <typename T>
void updateMatrixElement(std::vector<std::vector<T>>& data, size_t row, size_t col, T value, size_t size) {
    if (row >= size || col >= size) {
        throw std::out_of_range("Index out of bounds");
    }
    data[row][col] = value;
}

int main() {
    try {
        std::string filename;
        std::cout << "Enter the filename: ";
        std::cin >> filename;

        int type_flag;
        size_t n;
        
        // First read just the type flag and size
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file");
        }
        file >> n >> type_flag;
        file.close();

        if (type_flag == 0) {
            Matrix<int> mat1(n), mat2(n);
            readMatricesFromFile(filename, mat1, mat2);

            std::cout << "\nMatrix 1:\n";
            mat1.display();
            std::cout << "\nMatrix 2:\n";
            mat2.display();

            // Matrix addition
            std::cout << "\nMatrix 1 + Matrix 2:\n";
            (mat1 + mat2).display();

            // Matrix multiplication
            std::cout << "\nMatrix 1 * Matrix 2:\n";
            (mat1 * mat2).display();

            // Diagonal sums
            std::cout << "\nMatrix 1 main diagonal sum: " << mat1.sumMainDiagonal() << '\n';
            std::cout << "Matrix 1 secondary diagonal sum: " << mat1.sumSecondaryDiagonal() << '\n';

            // Swap rows
            size_t row1, row2;
            std::cout << "\nEnter two row indices to swap in Matrix 1 (0-based): ";
            std::cin >> row1 >> row2;
            mat1.swapRows(row1, row2);
            std::cout << "Matrix 1 after swapping rows " << row1 << " and " << row2 << ":\n";
            mat1.display();

            // Swap columns
            size_t col1, col2;
            std::cout << "\nEnter two column indices to swap in Matrix 1 (0-based): ";
            std::cin >> col1 >> col2;
            mat1.swapColumns(col1, col2);
            std::cout << "Matrix 1 after swapping columns " << col1 << " and " << col2 << ":\n";
            mat1.display();

            // Update element
            size_t row, col;
            int value;
            std::cout << "\nEnter row, column, and new value to update in Matrix 1: ";
            std::cin >> row >> col >> value;
            mat1.updateElement(row, col, value);
            std::cout << "Matrix 1 after update:\n";
            mat1.display();

            // Demonstrate polymorphic functions
            std::vector<std::vector<int>> intData = {{1, 2}, {3, 4}};
            std::cout << "\nDemonstrating polymorphic swap rows:\nBefore:\n";
            for (const auto& r : intData) {
                for (auto v : r) std::cout << v << ' ';
                std::cout << '\n';
            }
            polymorphicSwapRows(intData, 0, 1, 2);
            std::cout << "After swapping rows 0 and 1:\n";
            for (const auto& r : intData) {
                for (auto v : r) std::cout << v << ' ';
                std::cout << '\n';
            }

        } else if (type_flag == 1) {
            Matrix<double> mat1(n), mat2(n);
            readMatricesFromFile(filename, mat1, mat2);

            std::cout << "\nMatrix 1:\n";
            mat1.display();
            std::cout << "\nMatrix 2:\n";
            mat2.display();

            // Matrix addition
            std::cout << "\nMatrix 1 + Matrix 2:\n";
            (mat1 + mat2).display();

            // Matrix multiplication
            std::cout << "\nMatrix 1 * Matrix 2:\n";
            (mat1 * mat2).display();

            // Diagonal sums
            std::cout << "\nMatrix 1 main diagonal sum: " << mat1.sumMainDiagonal() << '\n';
            std::cout << "Matrix 1 secondary diagonal sum: " << mat1.sumSecondaryDiagonal() << '\n';

            // Swap rows
            size_t row1, row2;
            std::cout << "\nEnter two row indices to swap in Matrix 1 (0-based): ";
            std::cin >> row1 >> row2;
            mat1.swapRows(row1, row2);
            std::cout << "Matrix 1 after swapping rows " << row1 << " and " << row2 << ":\n";
            mat1.display();

            // Swap columns
            size_t col1, col2;
            std::cout << "\nEnter two column indices to swap in Matrix 1 (0-based): ";
            std::cin >> col1 >> col2;
            mat1.swapColumns(col1, col2);
            std::cout << "Matrix 1 after swapping columns " << col1 << " and " << col2 << ":\n";
            mat1.display();

            // Update element
            size_t row, col;
            double value;
            std::cout << "\nEnter row, column, and new value to update in Matrix 1: ";
            std::cin >> row >> col >> value;
            mat1.updateElement(row, col, value);
            std::cout << "Matrix 1 after update:\n";
            mat1.display();

            // Demonstrate polymorphic functions
            std::vector<std::vector<double>> doubleData = {{1.1, 2.2}, {3.3, 4.4}};
            std::cout << "\nDemonstrating polymorphic swap columns:\nBefore:\n";
            for (const auto& r : doubleData) {
                for (auto v : r) std::cout << v << ' ';
                std::cout << '\n';
            }
            polymorphicSwapColumns(doubleData, 0, 1, 2);
            std::cout << "After swapping columns 0 and 1:\n";
            for (const auto& r : doubleData) {
                for (auto v : r) std::cout << v << ' ';
                std::cout << '\n';
            }
        } else {
            throw std::runtime_error("Invalid type flag in file");
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
