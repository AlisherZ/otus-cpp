#include "DBCluster.h"

namespace join_server {

  DBCluster& DBCluster::Instance() {
    static DBCluster instance;
    return instance;
  }

  Response DBCluster::executeQuery(Request req) {
    std::size_t id = req.getId();
    addQuery(req);
    return getAnswerQuery(id);
  }
  
  void DBCluster::addQuery(Request req) {
    queries.push(req);
  }

  Response DBCluster::getAnswerQuery(std::size_t id) {
    Response value(id);
    while(answers.pop(value)) {
        if(value.getId() == id) {
          return value;
        }
        answers.push(value);
    }
    while(answers.tryPop(value)) {
        if(value.getId() == id) {
          return value;
        }
        answers.push(value);
    }
    return Response(id, "ERR something went wrong");
  }

  size_t DBCluster::openConnection() {
    std::unique_lock<std::mutex> lck{m_connection};
    std::size_t id = last_connection++;
    return id;
  }

  DBCluster::~DBCluster() {
    queries.stop();
    connectionDB.get();
  }

  DBCluster::DBCluster() {
    auto func = [this]() {
        Request value;
        // make a waiting pop from the queue
        while(queries.pop(value)) {
          std::string res = query(value) + "\n";
          Response rp(value.getId(), res);
          answers.push(rp);
        }
        while(queries.tryPop(value)) {
          std::string res = query(value) + "\n";
          Response rp(value.getId(), res);
          answers.push(rp);
        }
        answers.stop();
    };
    
    connectionDB = std::async(std::launch::async, func);
  }

  std::string DBCluster::query(Request req) {
    switch(req.getType()) {
      case query_type::insert: {
        InsertParams params(req.getParams());
        return insert(params);
      }
      case query_type::get: {
        TruncateParams params(req.getParams());
        return get(params);
      }
    }
    return "Err incomplete operation";
  }

  std::string DBCluster::insert(InsertParams params) {
    if(params.getName() == "A") {
      return insert(tableA, params);
    }
    if(params.getName() == "B") {
      return insert(tableB, params);
    }
    return "ERR no table " + params.getName();
  }

  std::string DBCluster::insert(Table<Row>& table, InsertParams params) {
    if(table.insert(params.getRow()) == 1) {
      return "OK";
    }
    else {
      return "ERR duplicate " + std::to_string(params.getRow().getId());
    }
  }

  std::string DBCluster::get(TruncateParams params) {
    if(params.getName() == "A") {
      return rowsToString<Row>(tableA.get());
    }
    if(params.getName() == "B") {
      return rowsToString<Row>(tableB.get());
    }
    return "ERR no table " + params.getName();
  }

}
