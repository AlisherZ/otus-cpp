#include "commbulk.h"

CommandBulk::timestamp CommandBulk::getTimeCreated() {
    return timeCreated;
}

void CommandBulk::AddCommand(std::string comm) {
    if(IsFull()) {
        return;
    }
    if(commands.size() == 0) {
        timeCreated = std::chrono::system_clock::now().time_since_epoch();
    }
    commands.push_back(comm);
}

void CommandBulk::clear() {
    commands.clear();
}

std::size_t CommandBulk::size() {
    return commands.size();
}

bool StaticCommandBulk::IsFull() {
    return commands.size() == max_capacity;
}

bool DynamicCommandBulk::IsFull() {
    return false;
}
