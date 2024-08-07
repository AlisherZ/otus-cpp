#include <boost/program_options.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/compute/detail/sha1.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/crc.hpp>

#include <iostream>
#include <set>

#include "asio_async_server.h"

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
        {
        std::cerr << "Usage: join_server <port>\n";
        return 1;
        }

        boost::asio::io_context io_context;

        join_server::server server(io_context, std::atoi(argv[1]));

        io_context.run();
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Exception: " << ex.what() << "\n";
    }

    return 0;
}
