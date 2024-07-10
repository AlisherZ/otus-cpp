#include <iostream>
#include "util.hpp"

int main(int, char **)
{
    /*Matrix<int, -1> matrix;
    matrix[100][100] = 314;
    std::cout << matrix[100][100] << std::endl;
    ((matrix[100][100] = 114) = -1);
    std::cout << matrix[100][100] << std::endl;
    std::cout << matrix.size() << std::endl;*/
    std::cout << "Start" << std::endl;

    Row<int, -1> row;
    row[100] = 314;
    row[34] = 73;
    row[0] = 33;
    std::cout << row.size() << std::endl;
    for(auto it : row) {
        std::cout << it.first << ' ' << it.second << std::endl;
    }
    row[1] = -1;
    ((row[0] = -1) = 9);
    std::cout << row.size() << std::endl;
    for(auto it : row) {
        std::cout << it.first << ' ' << it.second << std::endl;
    }
    std::cout << row[3] << std::endl;
    row[34] = -1;
    for(auto it : row) {
        std::cout << it.first << ' ' << it.second << std::endl;
    }

    Matrix2D<int, -1> matrix;
    matrix[100][100] = 314;
    matrix[34][23] = 14;
    std::cout << matrix[100][100] << std::endl;
    std::cout << matrix[34][23] << std::endl;
    std::cout << matrix.size() << std::endl;
    for(auto it : matrix) {
        std::cout << it.first << ' ' << it.second.size() << ':' << std::endl;
        for(auto it1 : it.second) {
            std::cout << it1.first << ' ' << it1.second << std::endl;
        }
    }
    ((matrix[100][100] = 114) = -1);
    std::cout << matrix[34][23] << std::endl;
    std::cout << matrix[100][100] << std::endl;
    std::cout << matrix.size() << std::endl;
    for(auto it : matrix) {
        std::cout << it.first << ' ' << it.second.size() << ':' << std::endl;
        for(auto it1 : it.second) {
            std::cout << it1.first << ' ' << it1.second << std::endl;
        }
    }
    matrix[0][0] = -1;
    std::cout << matrix[34][23] << std::endl;
    std::cout << matrix[0][0] << std::endl;
    std::cout << matrix.size() << std::endl;
    for(auto it : matrix) {
        std::cout << it.first << ' ' << it.second.size() << ':' << std::endl;
        for(auto it1 : it.second) {
            std::cout << it1.first << ' ' << it1.second << std::endl;
        }
    }
    ((matrix[1][1] = 12) = -1) = 37;
    std::cout << matrix[34][23] << std::endl;
    std::cout << matrix[1][1] << std::endl;
    std::cout << matrix.size() << std::endl;
    for(auto it : matrix) {
        std::cout << it.first << ' ' << it.second.size() << ':' << std::endl;
        for(auto it1 : it.second) {
            std::cout << it1.first << ' ' << it1.second << std::endl;
        }
    }

    return 0;
}
