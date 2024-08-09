#include <algorithm>

#include <iostream>

#include "row.h"

namespace join_server {

  std::string RowBase::getTitles() {
    if(order.size() == 0) {
      return "";
    }
    std::string res = "";
    bool first = true;
    for(auto key : order) {
      if(first) {
        first = false;
      }
      else {
        res+= " | ";
      }
      res+= key;
    }
    return res;
  }

  std::string RowBase::getLine() {
    if(row.size() == 0) {
      return "";
    }
    std::string res = "";
    bool first = true;
    for(auto key : order) {
      if(first) {
        first = false;
      }
      else {
        res+= "+";
      }
      res+= std::string(key.size() + 1, '-');
    }
    return res;
  }

  std::string RowBase::getValues() {
    if(row.size() == 0) {
      return "";
    }
    std::string res = "";
    bool first = true;
    for(auto key : order) {
      if(first) {
        first = false;
      }
      else {
        res+= " | ";
      }
      res+= row[key];
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

  std::vector<UnionRow> intersection(std::vector<Row> rows1, std::vector<Row> rows2) {
    std::sort(rows1.begin(), rows1.end(), [](Row a, Row b) { return a.getId() < b.getId(); });
    std::sort(rows2.begin(), rows2.end(), [](Row a, Row b) { return a.getId() < b.getId(); });
    std::vector<UnionRow> result;
    for(std::size_t i = 0, j = 0;i < rows1.size();i++) {
      for(;(j < rows2.size()) && (rows1[i].getId() > rows2[j].getId());j++);
      if((j < rows2.size()) && (rows1[i].getId() == rows2[j].getId())) {
        result.emplace_back(rows1[i].getId(), rows1[i].getName(), rows2[j].getName());
      }
    }
    return result;
  }

  std::vector<UnionRow> symmDiff(std::vector<Row> rows1, std::vector<Row> rows2) {
    std::sort(rows1.begin(), rows1.end(), [](Row a, Row b) { return a.getId() < b.getId(); });
    std::sort(rows2.begin(), rows2.end(), [](Row a, Row b) { return a.getId() < b.getId(); });
    std::vector<UnionRow> result;
    std::size_t i = 0, j = 0;
    for(;(i < rows1.size()) && (j < rows2.size());) {
      if(rows1[i].getId() < rows2[j].getId()) {
        result.emplace_back(rows1[i].getId(), rows1[i].getName(), "");
        i++;
      }
      else if(rows1[i].getId() > rows2[j].getId()) {
        result.emplace_back(rows2[j].getId(), "", rows2[j].getName());
        j++;
      }
      else {
        i++;
        j++;
      }
    }
    for(;i < rows1.size();i++) {
      result.emplace_back(rows1[i].getId(), rows1[i].getName(), "");
    }
    for(;j < rows2.size();j++) {
      result.emplace_back(rows2[j].getId(), "", rows2[j].getName());
    }
    return result;
  }
}