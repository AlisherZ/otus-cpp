#pragma once

#include <boost/asio.hpp>

#include <cstdlib>
#include <memory>
#include <utility>

#include "DBCluster.h"

namespace join_server {

  class session : public std::enable_shared_from_this<session>
  {
  public:
    session(boost::asio::ip::tcp::socket socket) : socket_(std::move(socket)) {  }

    void start();

  private:
    void do_read();
    void do_write(std::size_t length);

    boost::asio::ip::tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
    std::size_t id;
  };

  class server
  {
  public:
    server(boost::asio::io_context& io_context, short port) 
      : acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
      {
        do_accept();
      }

  private:
    void do_accept();

    boost::asio::ip::tcp::acceptor acceptor_;
  };

}
