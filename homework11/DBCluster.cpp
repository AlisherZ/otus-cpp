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

  Response DBCluster::getResponse(Request req) {
    try {
      std::string res = query(req);
      return Response(req.getId(), res);
    }
    catch(const std::invalid_argument& err) {
      std::string msg = err.what();
      return Response(req.getId(), "ERR " + msg);
    }
    catch(...) {
      return Response(req.getId(), "ERR Something went wrong");
    }
  }

  DBCluster::DBCluster() {
    auto func = [this]() {
        Request value;
        // make a waiting pop from the queue
        while(queries.pop(value)) {
          answers.push(getResponse(value));
        }
        while(queries.tryPop(value)) {
          answers.push(getResponse(value));
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
      case query_type::truncate: {
        TruncateParams params(req.getParams());
        return truncate(params);
      }
      case query_type::intersection: {
        return intersect();
      }
      case query_type::symmetric_difference: {
        return symmetricalDifference();
      }
    }
    throw std::exception("incomplete operation");
  }

  std::string DBCluster::insert(InsertParams params) {
    if(params.getName() == "A") {
      return insert(tableA, params);
    }
    if(params.getName() == "B") {
      return insert(tableB, params);
    }
    throw std::invalid_argument("no table " + params.getName());
  }

  std::string DBCluster::insert(Table<Row>& table, InsertParams params) {
    if(table.insert(params.getRow()) == 1) {
      return "OK";
    }
    throw std::invalid_argument("duplicate " + std::to_string(params.getRow().getId()));
  }

  std::string DBCluster::get(TruncateParams params) {
    if(params.getName() == "") {
      throw std::invalid_argument("format must be GET [table_name]");
    }
    if(params.getName() == "A") {
      return rowsToString<Row>(tableA.get());
    }
    if(params.getName() == "B") {
      return rowsToString<Row>(tableB.get());
    }
    throw std::invalid_argument("no table " + params.getName());
  }

  std::string DBCluster::truncate(TruncateParams params) {
    if(params.getName() == "") {
      throw std::invalid_argument("format must be TRUNCATE [table_name]");
    }
    if(params.getName() == "A") {
      tableA.clear();
      return "OK";
    }
    if(params.getName() == "B") {
      tableB.clear();
      return "OK";
    }
    throw std::invalid_argument("no table " + params.getName());
  }

  std::string DBCluster::intersect() {
    return rowsToString<UnionRow>(intersection(tableA.get(), tableB.get()));
  }

  std::string DBCluster::symmetricalDifference() {
    return rowsToString<UnionRow>(symmDiff(tableA.get(), tableB.get()));
  }

}
