#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "commbulk.h"

class Printer
{
public:
    void printToStream(CommandBulk& bulk, std::ostream& out);
    virtual void print(CommandBulk& bulk) = 0;
    virtual ~Printer() = default;
};

class ConsolePrinter : public Printer
{
public:
    void print(CommandBulk& bulk) override;
};

class FilePrinter : public Printer
{
public:
    void print(CommandBulk& bulk) override;
};
