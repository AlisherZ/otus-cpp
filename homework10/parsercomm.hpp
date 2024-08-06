#pragma once

#include <future>
#include <map>
#include <memory>
#include <vector>

#include "printer.hpp"
#include "waiting_queue.hpp"

#include <iostream>

namespace bulk_server {

    template<typename T>
    class ParserCommands
    {
    public:
        ParserCommands() : statBulk(), dynBulk(), isDynamic(), printers() {};
        ParserCommands(std::size_t capacity) : statBulk(capacity), dynBulk(), isDynamic(), printers() {};
        std::uint64_t getId() { return id_thread; };
        void setId(std::uint64_t new_id) { id_thread = new_id; };
        size_t openConnection() {
            std::unique_lock<std::mutex> lck{m_connection};
            num_connection++;
            std::size_t id = last_connection++;
            isDynamic[id] = 0;
            dynBulk[id].clear();
            return id;
        }
        void ParseCommand(T command, std::size_t id) {
            //std::unique_lock<std::mutex> lck1{m_console};
            //std::cout << command << std::endl;
            if(command == "") {
                return;
            }
            if(command == "{") {
                if(isStatic(id)) {
                    std::unique_lock<std::mutex> lck{m_statBulk};
                    ExecuteStatBulk();
                }
                isDynamic[id]++;
                return;
            }
            if(command == "}") {
                isDynamic[id]--;
                if(isStatic(id)) {
                    ExecuteDynBulk(id);
                }
                return;
            }
            if(isStatic(id)) {
                std::unique_lock<std::mutex> lck{m_statBulk};
                statBulk.AddCommand(command);
                if(statBulk.IsFull()) {
                    ExecuteStatBulk();
                }
            }
            else {
                dynBulk[id].AddCommand(command);
                if(dynBulk[id].IsFull()) {
                    ExecuteDynBulk(id);
                }
            }
        };
        bool isStatic(std::size_t id) { return isDynamic[id] == 0; };
        void ExecuteBulk(std::size_t id) {
            if(isStatic(id)) 
            {
                std::unique_lock<std::mutex> lck{m_statBulk};
                ExecuteBulk(statBulk); 
            }
            else {
                ExecuteBulk(dynBulk[id]);
            }
        };
        void closeConnection(std::size_t id) {
            std::unique_lock<std::mutex> lck{m_connection};
            num_connection--;
            dynBulk.erase(id);
            if(num_connection == 0) {
                std::unique_lock<std::mutex> lck1{m_statBulk};
                ExecuteStatBulk();
            }
        }
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
            std::unique_lock<std::mutex> lck{m_statBulk};
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
        void ExecuteDynBulk(std::size_t id) { ExecuteBulk(dynBulk[id]); };
        template<typename ...Args>
        void AddPrinter(printer_type type, WaitingQueue<std::shared_ptr<CommandBulk<T> > >& queue, Args... args) {
            auto func = [this](printer_type type, WaitingQueue<std::shared_ptr<CommandBulk<T> > >& queue, Args... args) {
                std::shared_ptr<CommandBulk<T> > value;
                auto printer = getPrinter<T>(type, args...);
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
            AddPrinter(printer_type::file, fileQueue, id_thread, id_file++);
        };
        std::mutex m_statBulk;
        std::mutex m_connection;
        std::mutex m_console;
        std::size_t last_connection = 0;
        std::size_t num_connection = 0;
        std::size_t last_printer = 0;
        std::uint64_t id_thread = 0;
        std::uint64_t id_file = 0;
        StaticCommandBulk<T> statBulk;
        std::map<std::size_t, DynamicCommandBulk<T> > dynBulk;
        std::map<std::size_t, std::uint64_t> isDynamic;
        std::vector<std::future<void> > printers;
        WaitingQueue<std::shared_ptr<CommandBulk<T> > > consoleQueue;
        WaitingQueue<std::shared_ptr<CommandBulk<T> > > fileQueue;
    };

}
