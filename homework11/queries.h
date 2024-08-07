#pragma once

#include <string>
#include <vector>

#include "row.h"

namespace join_server {

  enum class query_type
  {
      insert,
      get,
      truncate,
      intersection,
      symmetric_difference
  };

  query_type getTypeFromString(std::string name);

  class Request
  {
  public:
    Request() : id(0), type(query_type::intersection), params() {};
    Request(std::size_t new_id, query_type new_type, std::string new_params) : id(new_id), type(new_type), params(new_params) {};
    Request(std::size_t new_id, std::string request);
    Request(const Request& req) : id(req.id), type(req.type), params(req.params) {};
    std::size_t getId();
    query_type getType();
    std::string getParams();
  private:
    std::size_t id;
    query_type type;
    std::string params;
  };

  class Response
  {
  public:
    Response() : id(0), message() {};
    Response(std::size_t new_id) : id(new_id), message() {};
    Response(std::size_t new_id, std::string new_message) : id(new_id), message(new_message) {};
    Response(const Response& resp) : id(resp.id), message(resp.message) {};
    std::size_t getId();
    std::string getMessage();
  private:
    std::size_t id;
    std::string message;
  };

  class Params
  {
  public:
    Params() {};
    Params(std::string t) {};
  };

  class InsertParams : Params
  {
  public:
    InsertParams() : table(), row() {};
    InsertParams(std::string new_table, Row new_row) : table(new_table), row(new_row) {};
    InsertParams(std::string params);
    std::string getName();
    Row getRow();
  private:
    std::string table;
    Row row;
  };

  class TruncateParams : Params
  {
  public:
    TruncateParams() : table() {};
    TruncateParams(std::string params) : table(params) {};
    std::string getName();
  private:
    std::string table;
  };



}
