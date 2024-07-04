#include "allocator.hpp"
#include "container.hpp"

#include <gtest/gtest.h>

/*TEST(MapTest, DeleteAllocatorAssertions) {
  std::map<int, int, std::less<int>, pool_allocator<std::pair<const int, int> > > m;
  m[0] = 1;
  m.erase(0);
  EXPECT_TRUE(m.size() == 0);
}*/

/*
TEST(MapTest, DeleteAssertions) {
  std::map<int, int> m;
  m[0] = 1;
  m.erase(0);
  EXPECT_TRUE(m.size() == 0);
}
*/

TEST(MyAllocatorTest, DeleteAllocatorAssertions) {
  pool_allocator<int> alloc;
  /*int *t = alloc.allocate(1);
  std::cout << "Allocated value " << *t << std::endl;
  alloc.deallocate(t, 1);*/
  EXPECT_TRUE(alloc.num_pools == 0);
}

/*
TEST(MyContainerTest, DeleteAllocatorAssertions) {
  MyContainer<int, pool_allocator<int> > c;
  //c.push_back(0);
  //c.eraseByIndex(0);
  EXPECT_TRUE(c.size() == 0);
}
*/

/*
TEST(MyContainerTest, DeleteAssertions) {
  MyContainer<int> c;
  c.push_back(0);
  c.eraseByIndex(0);
  EXPECT_TRUE(c.size() == 0);
}
*/