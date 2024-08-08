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

  DBCluster::DBCluster() : queries(), answers() {
    auto func = [this]() {
        Request value;
        // make a waiting pop from the queue
        while(queries.pop(value)) {
          
          Response rp(value.getId(), "OK");
          answers.push(rp);
        }
        while(queries.tryPop(value)) {
          Response rp(value.getId(), "OK");
          answers.push(rp);
        }
        answers.stop();
    };
    
    connectionDB = std::async(std::launch::async, func);
  }

  std::string DBCluster::get(TruncateParams params) {
    if(params.getName() == "A") {
      tableA.get();
    }
    if(params.getName() == "B") {
      tableB.get();
    }
    return "";
  }

}
