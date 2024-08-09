#pragma once

#include <string>
#include <vector>
#include <map>

namespace join_server {

  class RowBase
  {
  public:
    RowBase() {};
    RowBase(std::map<std::string, std::string> new_row, std::vector<std::string> new_order) : row(new_row), order(new_order) {};
    std::string getTitles();
    std::string getLine();
    std::string getValues();
  protected:
    std::string getValue(std::string key);
    void setValue(std::string key, std::string value);
  private:
    std::map<std::string, std::string> row;
    std::vector<std::string> order;
  };

  class Row : public RowBase
  {
  public:
    Row() : RowBase({{"id", "0"}, {"name", ""}}, {"id", "name"}) {};
    Row(int id, std::string name) : RowBase({{"id", std::to_string(id)}, {"name", name}}, {"id", "name"}) {};
    int getId();
    void setId(int id);
    std::string getName();
    void setName(std::string name);
    bool update(Row row);
  };

  class UnionRow : public RowBase
  {
  public:
    UnionRow() : RowBase({{"id", "0"}, {"A", ""}, {"B", ""}}, {"id", "A", "B"}) {};
    UnionRow(int id, std::string nameA, std::string nameB) : RowBase({{"id", std::to_string(id)}, {"A", nameA}, {"B", nameB}}, {"id", "A", "B"}) {};
    int getId();
    void setId(int id);
    std::string getNameA();
    void setNameA(std::string name);
    std::string getNameB();
    void setNameB(std::string name);
    bool update(UnionRow row);
  };
  
  std::vector<UnionRow> intersection(std::vector<Row> rows1, std::vector<Row> rows2);
  std::vector<UnionRow> symmDiff(std::vector<Row> rows1, std::vector<Row> rows2);

}

