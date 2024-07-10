#include "commbulk.h"

CommandBulk::timestamp CommandBulk::getTimeCreated() {
    return timeCreated;
}

void CommandBulk::clear() {
    commands.clear();
}

std::size_t CommandBulk::size() {
    return commands.size();
}

void StaticCommandBulk::AddCommand(std::string comm) {
    if(IsFull()) {
        return;
    }
    if(commands.size() == 0) {
        timeCreated = std::chrono::system_clock::now().time_since_epoch();
    }
    commands.push_back(comm);
}

bool StaticCommandBulk::IsFull() {
    return commands.size() == max_capacity;
}
