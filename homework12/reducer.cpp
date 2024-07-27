#include <iostream>
#include <string>
#include "util.hpp"

int main(int, char **)
{
    size_t count = 0;
    double sum = 0;
    double var = 0;
    std::string line;
    while (std::getline(std::cin, line))
    {
        auto cut_row = split(split(split(line, '\t')[1], '(')[1], ')')[0];
        auto row = split(cut_row, ',');
        count+= std::stoi(row[0]);
        sum+= std::stoi(row[1]);
        var+= std::stoi(row[1]) * std::stoi(row[1]);
    }
    std::cout << sum / count <<"\t" << (var / count) - (sum / count) * (sum / count) << std::endl;
    return 0;
}
