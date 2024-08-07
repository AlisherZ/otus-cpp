#pragma once

#include <string>
#include <vector>

#include <iostream>

namespace join_server {

  template<typename T>
  class Table
  {
  public:
    Table<T>() {};
    Table<T>(std::vector<T> new_table) : table(new_table) {};
    auto get() {
      std::vector<T> result(table);
      return result;
    };
    T get(int id) {
      for(auto row : table) {
        if(row.getId() == id) {
          return row;
        }
      }
    };
    int insert(T new_row) {
      for(auto row : table) {
        if(row.getId() == new_row.getId()) {
          return 0;
        }
      }
      table.push_back(new_row);
      return 1;
    }
    int update(T new_row) {
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
    void clear() {
      table.clear();
    }
  private:
    std::vector<T> table;
  };

  template<typename T>
  std::string rowsToString(std::vector<T> rows) {
    if(rows.size() == 0) {
      return "The table was empty";
    }
    std::string res = rows[0].getTitles() + "\n" + rows[0].getLine();
    for(auto row : rows) {
      res+= "\n" + row.getValues();
    }
    return res;
  }

}
