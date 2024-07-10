#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "printer.h"

int main(int, char **)
{
    try
    {
        StaticCommandBulk block(3);
        ConsolePrinter p;
        FilePrinter p1;

        for(std::string line; std::getline(std::cin, line);)
        {
            block.AddCommand(line);
            if(block.IsFull()) {
                p.print(block);
                p1.print(block);
                block.clear();
            }
        }
        if(block.size() != 0) {
            p.print(block);
            p1.print(block);
            block.clear();
        }
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
