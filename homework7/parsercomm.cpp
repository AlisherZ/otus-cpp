#pragma once

#include "parsercomm.h"

void ParserCommands::ParseCommand(std::string command) {
    if(command == "{") {
        if(isStatic()) {
            ExecuteStatBulk();
        }
        isDynamic++;
        return;
    }
    if(command == "}") {
        isDynamic--;
        if(isStatic()) {
            ExecuteDynBulk();
        }
        return;
    }
    if(isStatic()) {
        statBulk.AddCommand(command);
        if(statBulk.IsFull()) {
            ExecuteStatBulk();
        }
    }
    else {
        dynBulk.AddCommand(command);
        if(dynBulk.IsFull()) {
            ExecuteDynBulk();
        }
    }
}

bool ParserCommands::isStatic() {
    return isDynamic == 0;
}

void ParserCommands::ExecuteBulk() {
    if(isStatic()) {
        ExecuteBulk(statBulk);
    }
    else {
        ExecuteBulk(dynBulk);
    }
}

void ParserCommands::ExecuteStatBulk() {
    ExecuteBulk(statBulk);
}

void ParserCommands::ExecuteDynBulk() {
    ExecuteBulk(dynBulk);
}

void ParserCommands::ExecuteBulk(CommandBulk& bulk) {
    for(auto printer : printers) {
        printer->print(bulk);
    }
    bulk.clear();
}

void ParserCommands::clear() {
    statBulk.clear();
    dynBulk.clear();
}

void ParserCommands::AddPrinter(std::shared_ptr<Printer> p) {
    printers.push_back(p);
}
