#pragma once

#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>

#include "commbulk.hpp"

namespace bulk_server {

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
                this->printToStream(bulk, std::cout);
            }
        }
    private:
        static std::mutex m_console;
    };

    template<typename T>
    std::mutex ConsolePrinter<T>::m_console;

    template<typename T>
    class FilePrinter : public Printer<T>
    {
    public:
        FilePrinter() : file_id(1) {};
        FilePrinter(std::uint64_t file) : file_id(file) {};
        void print(CommandBulk<T>& bulk) override {
            if(bulk.size() > 0) {
                std::string name = "bulk\\bulk" + std::to_string(bulk.getTimeCreated().count()) + "_" + std::to_string(bulk.getId()) + "_" + std::to_string(file_id) + ".txt";
                std::ofstream fout(name);
                this->printToStream(bulk, fout);
            }
        }
    private:
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
            case printer_type::console : {
                if constexpr(std::is_constructible_v<ConsolePrinter<T>, Args...>) {
                    return std::make_unique<ConsolePrinter<T> >(args...) ;
                }
                else {
                    throw std::invalid_argument( "received non-existent type of printer" );
                }
            }
            case printer_type::file : {
                if constexpr(std::is_constructible_v<FilePrinter<T>, Args...>) {
                    return std::make_unique<FilePrinter<T> >(args...) ;
                }
                else {
                    throw std::invalid_argument( "received non-existent type of printer" );
                }
            }
        };
        throw std::invalid_argument( "received non-existent type of printer" );
    };

}
