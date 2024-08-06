#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>

namespace bulk_server {

    template<typename T>
    class CommandBulk
    {
        using timestamp = std::chrono::duration<std::uint64_t, std::nano>;
    public:
        void AddCommand(T comm) {
            if(IsFull()) {
                return;
            }
            if(commands.size() == 0) {
                timeCreated = std::chrono::system_clock::now().time_since_epoch();
            }
            commands.push_back(comm);
        }
        virtual bool IsFull() = 0;
        void clear() { commands.clear(); }
        std::size_t size() { return commands.size(); }
        timestamp getTimeCreated() { return timeCreated; }
        auto begin() { return commands.begin(); };
        auto end() { return commands.end(); };
        virtual CommandBulk* clone() const = 0;
        virtual std::size_t getId() = 0;
    private:
        timestamp timeCreated;
        std::vector<T> commands;
    };

    template<typename T>
    class StaticCommandBulk : public CommandBulk<T>
    {
    public:
        StaticCommandBulk() : CommandBulk<T>(), max_capacity(1) {};
        StaticCommandBulk(std::size_t capacity) : CommandBulk<T>(), max_capacity(capacity) {};
        bool IsFull() override { return this->size() == max_capacity; };
        CommandBulk<T>* clone() const { return new StaticCommandBulk<T>(*this); };
        std::size_t getId() override { return 0; };
    private:
        std::size_t max_capacity;
    };

    template<typename T>
    class DynamicCommandBulk : public CommandBulk<T>
    {
    public:
        bool IsFull() override { return false; };
        CommandBulk<T>* clone() const { return new DynamicCommandBulk<T>(*this); };
        std::size_t getId() override { return id; };
        void setId(std::size_t new_id) { id = new_id; };
    private:
        std::size_t id;
    };

}
