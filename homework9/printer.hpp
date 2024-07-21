#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "commbulk.hpp"

namespace async {

    std::mutex m_console;

    template<typename T>
    class Printer
    {
    public:
        void printToStream(CommandBulk<T>& bulk, std::ostream& out) {
            out << "bulk:";
            bool first = true;
            for(auto comm : bulk) {
                if(first) {
                    first = false;
                }
                else {
                    out << ',';
                }
                out << ' ' << comm;
            }
            out << std::endl;
        }
        virtual void print(CommandBulk<T>& bulk) = 0;
        virtual ~Printer() = default;
    };

    template<typename T>
    class ConsolePrinter : public Printer<T>
    {
    public:
        void print(CommandBulk<T>& bulk) override {
            if(bulk.size() > 0) {
                std::unique_lock<std::mutex> lck{m_console};
                printToStream(bulk, std::cout);
            }
        }
    };

    template<typename T>
    class FilePrinter : public Printer<T>
    {
    public:
        FilePrinter() : thread_id(1), file_id(1) {};
        FilePrinter(std::uint64_t thread, std::uint64_t file) : thread_id(thread), file_id(file) {};
        void print(CommandBulk<T>& bulk) override {
            if(bulk.size() > 0) {
                std::string name = "bulk\\bulk" + std::to_string(bulk.getTimeCreated().count()) + "_" + std::to_string(thread_id) + "_" + std::to_string(file_id) + ".txt";
                std::ofstream fout(name);
                printToStream(bulk, fout);
            }
        }
    private:
        std::uint64_t thread_id;
        std::uint64_t file_id;
    };

    enum class printer_type
    {
        console,
        file
    };

    template<typename T, typename ...Args>
    static std::unique_ptr<Printer<T> > getPrinter(printer_type type, Args... args)
    {
        switch (type)
        {
            case printer_type::console : return std::make_unique<ConsolePrinter<T> >(args...) ;
            case printer_type::file : return std::make_unique<FilePrinter<T> >(args...);
        };
        throw std::invalid_argument( "received non-existent type of printer" );
    };

}