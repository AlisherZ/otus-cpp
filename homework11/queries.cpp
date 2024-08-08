#include <stdexcept>
#include <string>
#include <vector>

#include "queries.h"

namespace join_server {

  query_type getTypeFromString(std::string op) {
    if(op == "INSERT") {
      return query_type::insert;
    }
    if(op == "GET") {
      return query_type::get;
    }
    if(op == "TRUNCATE") {
      return query_type::truncate;
    }
    if(op == "INTERSECTION") {
      return query_type::intersection;
    }
    if(op == "SYMMETRIC_DIFFERENCE") {
      return query_type::symmetric_difference;
    }
    throw std::invalid_argument( "received non-existent type of query" );
  }

  Request::Request(std::size_t new_id, std::string request) {
    id = new_id;
    auto parts = split(request, ' ');
    type = getTypeFromString(parts[0]);
    params = request.substr(parts[0].size() + 1);
  }

  std::size_t Request::getId() {
    return id;
  }

  query_type Request::getType() {
    return type;
  }
  
  std::string Request::getParams() {
    return params;
  }

  std::size_t Response::getId() {
    return id;
  }

  std::string Response::getMessage() {
    return message;
  }

  InsertParams::InsertParams(std::string params) {
    auto parts = split(params, ' ');
    table = parts[0];
    row = Row(std::stoi(parts[1]), parts[2]);
  }
  
  std::string InsertParams::getName() {
    return table;
  }
  
  Row InsertParams::getRow() {
    return row;
  }

  std::string TruncateParams::getName() {
    return table;
  }

}
