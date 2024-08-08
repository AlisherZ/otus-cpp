#include <iostream>

#include "asio_async_server.h"

namespace join_server {

  void session::start() {
    id = DBCluster::Instance().openConnection();
    do_read();
  }

  void session::do_read() {
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
      [this, self](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
          std::string query{data_, length};
          std::cout << "receive " << length << "=" << query << std::endl;
          Request req(id, query);
          std::cout << "Start " << req.getId() << " request" << std::endl;
          Response resp = DBCluster::Instance().executeQuery(req);
          std::size_t length1 = resp.getMessage().size();
          resp.getMessage().copy(data_, length1);
          std::cout << "Finish " << resp.getId() << " request" << std::endl;
          do_write(length1);
        }
      });
  }

  void session::do_write(std::size_t length) {
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
      [this, self](boost::system::error_code ec, std::size_t /*length*/) {
        if (!ec) {
          do_read();
        }
      });
  }

  void server::do_accept() {
    acceptor_.async_accept(
      [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
        if (!ec) {
          std::make_shared<session>(std::move(socket))->start();
        }

        do_accept();
      });
  }

};
