#pragma once

#include <boost/asio.hpp>

#include <cstdlib>
#include <memory>
#include <utility>

#include "parsercomm.hpp"

namespace bulk_server {

  class session : public std::enable_shared_from_this<session>
  {
  public:
    session(boost::asio::ip::tcp::socket socket, ParserCommands<std::string>& new_parser) : socket_(std::move(socket)), parser(new_parser)  {  }

    void start();
    
    ~session() { parser.closeConnection(id); }

  private:
    void do_read();

    boost::asio::ip::tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
    std::size_t id;
    ParserCommands<std::string>& parser;
  };

  class server
  {
  public:
    server(boost::asio::io_context& io_context, short port) 
      : acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), parser(3) 
      {
        parser.AddConsolePrinters();
        do_accept();
      }
    server(boost::asio::io_context& io_context, short port, std::size_t capacity) 
      : acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), parser(capacity) 
      {
        parser.AddConsolePrinters();
        do_accept();
      }

  private:
    void do_accept();

    boost::asio::ip::tcp::acceptor acceptor_;
    ParserCommands<std::string> parser;
  };

}
