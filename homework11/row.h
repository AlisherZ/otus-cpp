#pragma once

#include <string>
#include <vector>

namespace join_server {

  class Row
  {
  public:
    Row() : id(0), name("") {};
    Row(int new_id, std::string new_name) : id(new_id), name(new_name) {};
    int getId();
    void setId(int id);
    std::string getName();
    void setName(std::string name);
    bool update(Row row);
  private:
    int id;
    std::string name;
  };

}
