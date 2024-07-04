#include "allocator.hpp"

#include <gtest/gtest.h>

TEST(MapTest, DeleteAllocatorAssertions) {
  std::map<int, int, std::less<int>, pool_allocator<std::pair<const int, int> > > m1;
  //m1[0] = 1;
  //m1.erase(0);
  EXPECT_TRUE(m1.size() == 0);
}

TEST(MapTest, DeleteAssertions) {
  std::map<int, int> m1;
  m1[0] = 1;
  m1.erase(0);
  EXPECT_TRUE(m1.size() == 0);
}
