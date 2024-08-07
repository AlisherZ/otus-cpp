#pragma once

#include <future>
#include <string>
#include <vector>

#include "queries.h"
#include "tableDAO.hpp"
#include "waiting_queue.hpp"

namespace join_server {

  using TableA = Table<Row>;
  using TableB = Table<Row>;

  class DBCluster
  {
  public:
    static DBCluster& Instance();
    void addQuery(Request req);
    Response getAnswerQuery(std::size_t id);
    size_t openConnection();
    ~DBCluster();
  private:
    DBCluster();
    WaitingQueue<Request> queries;
    WaitingQueue<Response> answers;
    std::future<void> connectionDB;
    std::mutex m_connection;
    std::size_t last_connection = 0;
  };

}
