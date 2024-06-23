#include "util.cpp"

#include <gtest/gtest.h>

TEST(ComparatorEqualTest, BasicEqualAssertions) {
  std::vector<std::vector<std::string> > ip_pool1 = { {"1", "2", "3", "4"}, {"5", "6", "7", "8"}, {"9", "10", "11", "12"} };
  std::vector<std::vector<std::string> > ip_pool2 = { {"1", "2", "3", "4"}, {"5", "6", "7", "8"}, {"9", "10", "11", "12"} };
  EXPECT_TRUE(comp_eq_pool(ip_pool1, ip_pool2));
}

TEST(ComparatorEqualTest, BasicNotEqualAssertions) {
  std::vector<std::vector<std::string> > ip_pool1 = { {"1", "2", "3", "4"}, {"5", "6", "7", "8"}, {"9", "10", "11", "12"} };
  std::vector<std::vector<std::string> > ip_pool2 = { {"1", "2", "3", "4"}, {"5", "7", "7", "8"}, {"9", "10", "11", "12"} };
  EXPECT_FALSE(comp_eq_pool(ip_pool1, ip_pool2));
}

TEST(ComparatorEqualTest, LineSizeNotEqualAssertions) {
  std::vector<std::vector<std::string> > ip_pool1 = { {"1", "2", "3", "4"}, {"5", "6", "7", "8"}, {"9", "10", "11", "12"} };
  std::vector<std::vector<std::string> > ip_pool2 = { {"1", "2", "3", "4"}, {"5", "6", "7", "8"}, {"9", "10", "11"} };
  EXPECT_FALSE(comp_eq_pool(ip_pool1, ip_pool2));
}

TEST(ComparatorEqualTest, SizeNotEqualAssertions) {
  std::vector<std::vector<std::string> > ip_pool1 = { {"1", "2", "3", "4"}, {"5", "6", "7", "8"}, {"9", "10", "11", "12"} };
  std::vector<std::vector<std::string> > ip_pool2 = { {"1", "2", "3", "4"}, {"5", "6", "7", "8"} };
  EXPECT_FALSE(comp_eq_pool(ip_pool1, ip_pool2));
}

TEST(ComparatorEqualTest, EmptyEqualAssertions) {
  std::vector<std::vector<std::string> > ip_pool1 = {};
  std::vector<std::vector<std::string> > ip_pool2 = {};
  EXPECT_TRUE(comp_eq_pool(ip_pool1, ip_pool2));
}


TEST(FilterTest, BasicAssertions) {
  std::vector<std::vector<std::string> > ip_pool1 = { {"1", "2", "3", "4"}, {"5", "6", "7", "8"}, {"9", "10", "11", "12"} };
  std::vector<std::vector<std::string> > ip_pool2 = { {"1", "2", "3", "4"} };
  EXPECT_TRUE(comp_eq_pool(filter(ip_pool1, 1, 2, 3, 4), ip_pool2));
}

TEST(FilterTest, BasicMutualAssertions) {
  std::vector<std::vector<std::string> > ip_pool1 = { {"1", "2", "3", "4"}, {"1", "2", "7", "8"}, {"9", "10", "11", "12"} };
  std::vector<std::vector<std::string> > ip_pool2 = { {"1", "2", "3", "4"}, {"1", "2", "7", "8"} };
  EXPECT_TRUE(comp_eq_pool(filter(ip_pool1, 1, 2), ip_pool2));
}

TEST(FilterTest, BasicEmptyAssertions) {
  std::vector<std::vector<std::string> > ip_pool1 = { {"1", "2", "3", "4"}, {"1", "2", "7", "8"}, {"9", "10", "11", "12"} };
  std::vector<std::vector<std::string> > ip_pool2 = {};
  EXPECT_TRUE(comp_eq_pool(filter(ip_pool1, 1, 2, 4), ip_pool2));
}

TEST(FilterTest, MoreValueForFilterAssertions) {
  std::vector<std::vector<std::string> > ip_pool1 = { {"1", "2", "3", "4"}, {"1", "2", "7", "8"}, {"9", "10", "11", "12"} };
  std::vector<std::vector<std::string> > ip_pool2 = { };
  EXPECT_TRUE(comp_eq_pool(filter(ip_pool1, 1, 2, 3, 4, 5), ip_pool2));
}

TEST(FilterTest, EmptyAssertions) {
  std::vector<std::vector<std::string> > ip_pool1 = {};
  std::vector<std::vector<std::string> > ip_pool2 = {};
  EXPECT_TRUE(comp_eq_pool(filter(ip_pool1), ip_pool2));
}

TEST(FilterAnyTest, BasicAssertions) {
  std::vector<std::vector<std::string> > ip_pool1 = { {"1", "2", "3", "4"}, {"5", "6", "7", "8"}, {"9", "10", "11", "12"} };
  std::vector<std::vector<std::string> > ip_pool2 = { {"1", "2", "3", "4"} };
  EXPECT_TRUE(comp_eq_pool(filter_any(ip_pool1, 4), ip_pool2));
}

TEST(FilterAnyTest, BasicMutualAssertions) {
  std::vector<std::vector<std::string> > ip_pool1 = { {"1", "2", "3", "4"}, {"1", "2", "7", "8"}, {"9", "10", "11", "12"} };
  std::vector<std::vector<std::string> > ip_pool2 = { {"1", "2", "3", "4"}, {"1", "2", "7", "8"} };
  EXPECT_TRUE(comp_eq_pool(filter_any(ip_pool1, 2), ip_pool2));
}

TEST(FilterAnyTest, BasicEmptyAssertions) {
  std::vector<std::vector<std::string> > ip_pool1 = { {"1", "2", "3", "4"}, {"5", "6", "7", "8"}, {"9", "10", "11", "12"} };
  std::vector<std::vector<std::string> > ip_pool2 = {};
  EXPECT_TRUE(comp_eq_pool(filter_any(ip_pool1, 13), ip_pool2));
}

TEST(FilterAnyTest, EmptyAssertions) {
  std::vector<std::vector<std::string> > ip_pool1 = {};
  std::vector<std::vector<std::string> > ip_pool2 = {};
  EXPECT_TRUE(comp_eq_pool(filter_any(ip_pool1, 1), ip_pool2));
}
