#pragma once

#include <memory>
#include "printer.h"

class ParserCommands
{
public:
    ParserCommands() : statBulk(), dynBulk(), isDynamic(false), printers() {};
    ParserCommands(std::size_t capacity) : statBulk(capacity), dynBulk(), isDynamic(false), printers() {};
    void ParseCommand(std::string command);
    bool isStatic();
    void ExecuteBulk();
    void clear();
    void AddPrinter(std::shared_ptr<Printer> p);
    ~ParserCommands();
private:
    StaticCommandBulk statBulk;
    DynamicCommandBulk dynBulk;
    std::uint64_t isDynamic;
    std::vector<std::shared_ptr<Printer> > printers;
    void ExecuteBulk(CommandBulk& bulk);
    void ExecuteStatBulk();
    void ExecuteDynBulk();
};