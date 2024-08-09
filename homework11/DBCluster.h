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
    Response executeQuery(Request req);
    size_t openConnection();
    ~DBCluster();
  private:
    void addQuery(Request req);
    Response getAnswerQuery(std::size_t id);
    std::string query(Request req);
    std::string insert(InsertParams params);
    std::string insert(Table<Row>& table, InsertParams params);
    std::string get(TruncateParams params);
    std::string truncate(TruncateParams params);
    std::string intersect();
    std::string symmetricalDifference();
    DBCluster();
    WaitingQueue<Request> queries;
    WaitingQueue<Response> answers;
    std::future<void> connectionDB;
    std::mutex m_connection;
    std::size_t last_connection = 0;
    TableA tableA;
    TableB tableB;
  };

}
