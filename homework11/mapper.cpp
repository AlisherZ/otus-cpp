#include <iostream>
#include <string>
#include "util.hpp"
#include <numeric>
#include <algorithm>

int main(int, char **)
{
    std::string line;
    while (std::getline(std::cin, line))
    {
        auto row = split(line, '"');
        if(row.size() > 1) {
            for(int i = 1;i < row.size();i+= 2) {
                std::replace(row[i].begin(), row[i].end(), ',', ';');
            }
            line = std::accumulate(
                std::next(row.begin()), 
                row.end(), 
                row[0],
                [](std::string a, std::string b) {
                    return a + '"' + b;
                }
            );
        }
        row = split(line, ',');
        std::cout << "(" << row[0] << ")\t(" << "1" << "," << row[9] << ")" << std::endl;
    }

    return 0;
}
