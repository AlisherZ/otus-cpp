#include <cassert>
#include <iostream>
#include "util.hpp"

int main(int, char **)
{
    /*
    Matrix<int, -1> matrix;
    assert(matrix.size() == 0);
    auto a = matrix[0][0];
    assert(a == -1);
    assert(matrix.size() == 0);
    matrix[100][100] = 314;
    assert(matrix.size() == 1);
    for(auto c : matrix) {
        std::size_t x;
        std::size_t y;
        int v;
        std::tie(x, y, v) = c;
        std::cout << x << y << v << std::endl;
    }
    */
    Matrix<int, 0> matrix;
    for(std::size_t i = 0;i < 10;i++) {
        matrix[i][i] = (int)i;
    }
    for(std::size_t i = 0;i < 10;i++) {
        matrix[9 - i][i] = (int)i;
    }
    for(std::size_t i = 1;i < 9;i++) {
        for(std::size_t j = 1;j < 9;j++) {
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << matrix.size() << std::endl;
    for(auto it : matrix) {
        std::size_t x;
        std::size_t y;
        int v;
        std::tie(x, y, v) = it;
        std::cout << "(" << x << ";" << y << "):" << v << std::endl;
    }
    //((matrix[100][100] = 314) = 0) = 217;

    return 0;
}
