#pragma once

#include <string>
#include <vector>
#include <map>

namespace join_server {

  class RowBase
  {
  public:
    RowBase() {};
    RowBase(std::map<std::string, std::string> new_row) : row(new_row) {};
    std::string getTitles();
    std::string getDivider();
    std::string getValues();
  protected:
    std::string getValue(std::string key);
    void setValue(std::string key, std::string value);
  private:
    std::map<std::string, std::string> row;
  };

  class Row : RowBase
  {
  public:
    Row() : RowBase({{"id", "0"}, {"name", ""}}) {};
    Row(int id, std::string name) : RowBase({{"id", std::to_string(id)}, {"name", name}}) {};
    int getId();
    void setId(int id);
    std::string getName();
    void setName(std::string name);
    bool update(Row row);
  };

  class UnionRow : RowBase
  {
  public:
    UnionRow() : RowBase({{"id", "0"}, {"A", ""}, {"B", ""}}) {};
    UnionRow(int id, std::string nameA, std::string nameB) : RowBase({{"id", std::to_string(id)}, {"A", nameA}, {"B", nameB}}) {};
    int getId();
    void setId(int id);
    std::string getNameA();
    void setNameA(std::string name);
    std::string getNameB();
    void setNameB(std::string name);
    bool update(UnionRow row);
  };
  
  std::string rowsToString(std::vector<Row> rows);
  std::string rowsToString(std::vector<UnionRow> rows);

}

