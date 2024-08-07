#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>

class CommandBulk
{
    using timestamp = std::chrono::duration<std::uint64_t, std::nano>;
public:
    void AddCommand(std::string comm);
    virtual bool IsFull() = 0;
    void clear();
    std::size_t size();
    timestamp getTimeCreated();
    auto begin() { return commands.begin(); };
	auto end() { return commands.end(); };
protected:
    timestamp timeCreated;
    std::vector<std::string> commands;
};

class StaticCommandBulk : public CommandBulk
{
public:
    StaticCommandBulk() : CommandBulk(), max_capacity(1) {};
    StaticCommandBulk(std::size_t capacity) : CommandBulk(), max_capacity(capacity) {};
    bool IsFull() override;
private:
    std::size_t max_capacity;
};

class DynamicCommandBulk : public CommandBulk
{
public:
    bool IsFull() override;
};
