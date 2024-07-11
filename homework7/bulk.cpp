#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "parsercomm.h"

int main(int argc, char ** argv)
{
    try
    {
        if(argc < 2) {
            std::cout << "Too few arguments." << std::endl;
            throw std::invalid_argument( "Needed maximum capacity of bulk." );
        }
        ParserCommands parser(std::atoi(argv[1]));
        parser.AddPrinter(std::make_shared<ConsolePrinter>());
        parser.AddPrinter(std::make_shared<FilePrinter>());

        for(std::string line; std::getline(std::cin, line);)
        {
            parser.ParseCommand(line);
        }
        parser.ExecuteStatBulk();
        parser.clear();
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
