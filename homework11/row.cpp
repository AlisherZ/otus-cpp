#include <algorithm>

#include "row.h"

namespace join_server {

  int Row::getId() {
    return id;
  }

  void Row::setId(int new_id) {
    id = new_id;
  }

  std::string Row::getName() {
    return name;
  }

  void Row::setName(std::string new_name) {
    name = new_name;
  }

  bool Row::update(Row row) {
    if(id == row.getId()) {
      name = row.getName();
      return true;
    }
    return false;
  }

}
