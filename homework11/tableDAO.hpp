#pragma once

#include <string>
#include <vector>

namespace join_server {

  template<typename T>
  class Table
  {
  public:
    Table<T>() : {};
    auto get() {
      std::array<Row, table.size()> res;
      std::copy(table.begin(), table.begin() + n, res.begin());
      return res;
    };
    T get(int id) {
      for(auto row : table) {
        if(row.getId() == id) {
          return row;
        }
      }
    };
    int insert(T row) {
      for(auto row : table) {
        if(row.getId() == row.getId()) {
          return 0;
        }
      }
      table.emplace_back(new_row);
      return 1;
    }
    int update(T row) {
      int cnt = 0;
      for(auto row : table) {
        if(row.update(new_row)) {
          cnt++;
        }
      }
      return cnt;
    }
    int remove(T row) {
      int cnt = 0;
      for(auto it = table.begin(); it != table.end();) {
        if(it.getId() == row.getId()) {
          it = table.erase(it);
          cnt++;
        }
        else {
          it++;
        }
      }
      return cnt;
    }
  private:
    std::vector<T> table;
  };

}