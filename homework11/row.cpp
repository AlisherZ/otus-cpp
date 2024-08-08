#include <algorithm>

#include "row.h"

namespace join_server {

  std::string RowBase::getTitles() {
    if(row.size() == 0) {
      return "";
    }
    std::string res = "";
    bool first = true;
    for(auto it = row.begin();it != row.end();it++) {
      if(first) {
        first = false;
      }
      else {
        res+= " | ";
      }
      res+= it->first;
    }
    return res;
  }

  std::string RowBase::getLine() {
    if(row.size() == 0) {
      return "";
    }
    std::string res = "";
    bool first = true;
    for(auto it = row.begin();it != row.end();it++) {
      if(first) {
        first = false;
      }
      else {
        res+= "+";
      }
      res+= std::string((it->first).size() + 1, '-');
    }
    return res;
  }

  std::string RowBase::getValues() {
    if(row.size() == 0) {
      return "";
    }
    std::string res = "";
    bool first = true;
    for(auto it = row.begin();it != row.end();it++) {
      if(first) {
        first = false;
      }
      else {
        res+= " | ";
      }
      res+= it->second;
    }
    return res;
  }

  std::string RowBase::getValue(std::string key) {
    return row[key];
  }

  void RowBase::setValue(std::string key, std::string value) {
    row[key] = value;
  }

  int Row::getId() {
    return std::stoi(getValue("id"));
  }

  void Row::setId(int id) {
    setValue("id", std::to_string(id));
  }

  std::string Row::getName() {
    return getValue("name");
  }

  void Row::setName(std::string name) {
    setValue("name", name);
  }

  bool Row::update(Row new_row) {
    if(getId() == new_row.getId()) {
      setValue("name", new_row.getName());
      return true;
    }
    return false;
  }

  int UnionRow::getId() {
    return std::stoi(getValue("id"));
  }

  void UnionRow::setId(int id) {
    setValue("id", std::to_string(id));
  }

  std::string UnionRow::getNameA() {
    return getValue("A");
  }

  void UnionRow::setNameA(std::string name) {
    setValue("A", name);
  }

  std::string UnionRow::getNameB() {
    return getValue("B");
  }

  void UnionRow::setNameB(std::string name) {
    setValue("B", name);
  }

  bool UnionRow::update(UnionRow new_row) {
    if(getId() == new_row.getId()) {
      setValue("A", new_row.getNameA());
      setValue("B", new_row.getNameB());
      return true;
    }
    return false;
  }

}
