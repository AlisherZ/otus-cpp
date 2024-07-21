#include <memory>
#include <string>
#include <vector>
#include "parsercomm.hpp"
#include "async.h"

#include <iostream>

namespace async {

    std::vector<std::shared_ptr<ParserCommands<std::string> > > parsers;
    std::uint64_t last_id = 0;

    std::vector<std::string> split(const std::string &str, char d)
    {
        std::vector<std::string> r;

        std::string::size_type start = 0;
        std::string::size_type stop = str.find_first_of(d);
        while(stop != std::string::npos)
        {
            r.push_back(str.substr(start, stop - start));

            start = stop + 1;
            stop = str.find_first_of(d, start);
        }

        r.push_back(str.substr(start));

        return r;
    }

    handle_t connect(std::size_t bulk) {
        auto& ref = parsers.emplace_back(std::make_shared<ParserCommands<std::string> >(bulk));
        ref->setId(last_id++);
        ref->AddConsolePrinters();
        ref->AddFilePrinters(2);
        return ref.get();
    }

    void receive(handle_t handle, const char *data, std::size_t size) {
        std::string str_data;
        str_data.assign(data, data + size);
        auto comands = split(str_data, '\n');
        auto parser_ptr = reinterpret_cast<ParserCommands<std::string>*>(handle);
        for(auto command : comands) {
            if(command != "") {
                parser_ptr->ParseCommand(command);
            }
        }
    }

    void disconnect(handle_t handle) {
        auto parser_ptr = static_cast<ParserCommands<std::string>*>(handle);
        for(auto it = parsers.begin(); it != parsers.end();) {
            if((*it)->getId() == parser_ptr->getId()) {
                it = parsers.erase(it);
            }
            else {
                it++;
            }
        }
    }

}
