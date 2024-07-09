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
    for(auto it : row) {
        std::cout << it.first << ' ' << it.second << std::endl;
    }

    return 0;
}
