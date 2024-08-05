#include <boost/asio.hpp>

#include <cstdlib>
#include <memory>
#include <utility>

#include "asio_async_server.h"
#include "util.h"

namespace bulk_server {

  //template<>
  //std::mutex ConsolePrinter<std::string>::m_console;

  void session::start()
  {
    id = parser.openConnection();
    do_read();
  }

  void session::do_read()
  {
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
          if (!ec)
          {
            std::string data = std::string{data_, length};
            auto commands = split(data, '\n');
            for(auto command : commands) {
              if((command.size() > 0) && (command[command.size() - 1] == '\r')) {
                command.pop_back();
              }
              parser.ParseCommand(command, id);
            }
            do_read();
          }
        });
  }

  void server::do_accept()
  {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket)
        {
          if (!ec)
          {
            std::make_shared<session>(std::move(socket), parser)->start();
          }

          do_accept();
        });
  }

}
