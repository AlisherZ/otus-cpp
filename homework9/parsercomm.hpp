#pragma once

#include <future>
#include <memory>
#include <vector>
#include "printer.hpp"
#include "waiting_queue.hpp"

#include <iostream>

namespace async {

    template<typename T>
    class ParserCommands
    {
    public:
        ParserCommands() : statBulk(), dynBulk(), isDynamic(false), printers() {};
        ParserCommands(std::size_t capacity) : statBulk(capacity), dynBulk(), isDynamic(false), printers() {};
        std::uint64_t getId() { return id; };
        void setId(std::uint64_t new_id) { id = new_id; };
        void ParseCommand(T command) {
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
        };
        bool isStatic() { return isDynamic == 0; };
        void ExecuteBulk() {
            if(isStatic()) 
            {
                ExecuteBulk(statBulk); 
            }
            else {
                ExecuteBulk(dynBulk);
            }
        };
        void clear() { 
            statBulk.clear();
            dynBulk.clear();
        };
        void AddConsolePrinters(std::size_t num = 1) {
            for(std::size_t i = 0;i < num;i++) {
                AddConsolePrinter();
            }
        };
        void AddFilePrinters(std::size_t num = 1) {
            for(std::size_t i = 0;i < num;i++) {
                AddFilePrinter();
            }
        };
        ~ParserCommands() {
            ExecuteStatBulk();
            consoleQueue.stop();
            fileQueue.stop();
            for(auto& printer : printers) {
                printer.get();
            }
            clear();
        };
    private:
        void ExecuteBulk(CommandBulk<T>& bulk) {
            std::shared_ptr<CommandBulk<T> > bulk_copy = std::shared_ptr<CommandBulk<T> >(bulk.clone());
            consoleQueue.push(bulk_copy);
            fileQueue.push(bulk_copy);
            bulk.clear();
        }
        void ExecuteStatBulk() { ExecuteBulk(statBulk); };
        void ExecuteDynBulk() { ExecuteBulk(dynBulk); };
        template<typename ...Args>
        void AddPrinter(printer_type type, WaitingQueue<std::shared_ptr<CommandBulk<T> > >& queue, Args... args) {
            auto func = [this](printer_type type, WaitingQueue<std::shared_ptr<CommandBulk<T> > >& queue, Args... args) {
                std::shared_ptr<CommandBulk<T> > value;
                auto printer = getPrinter<T>(type, args...);
                // make a waiting pop from the queue
                while(queue.pop(value)) {
                    printer->print(*value);
                }
                while(queue.tryPop(value)) {
                    printer->print(*value);
                }
            };
            
            auto future = std::async(std::launch::async, func, type, std::ref(queue), args...);
            printers.emplace_back(std::move(future));
        }
        void AddConsolePrinter() {
            AddPrinter(printer_type::console, consoleQueue);
        };
        void AddFilePrinter() {
            AddPrinter(printer_type::file, fileQueue, id, id_file++);
        };
        std::uint64_t id;
        std::uint64_t id_file = 0;
        StaticCommandBulk<T> statBulk;
        DynamicCommandBulk<T> dynBulk;
        std::uint64_t isDynamic;
        std::vector<std::future<void> > printers;
        WaitingQueue<std::shared_ptr<CommandBulk<T> > > consoleQueue;
        WaitingQueue<std::shared_ptr<CommandBulk<T> > > fileQueue;
    };

}