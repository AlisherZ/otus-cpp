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
        DynamicCommandBulk block1;
        ConsolePrinter p;
        FilePrinter p1;
        std::size_t isStatic = 0;

        for(std::string line; std::getline(std::cin, line);)
        {
            if(line == "{") {
                if(isStatic == 0) {
                    p.print(block);
                    p1.print(block);
                    block.clear();
                }
                isStatic++;
                continue;
            }
            if(line == "}") {
                isStatic--;
                if(isStatic == 0) {
                    p.print(block1);
                    p1.print(block1);
                    block1.clear();
                }
                continue;
            }
            if(isStatic > 0) {
                block1.AddCommand(line);
            }
            else {
                block.AddCommand(line);
                if(block.IsFull()) {
                    p.print(block);
                    p1.print(block);
                    block.clear();
                }
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
