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
  pool_allocator<int, 2> alloc;
  std::shared_ptr<int[]> tmp = std::shared_ptr<int[]>(new int[10]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
  std::cout << tmp[0] << std::endl;
  int *t = alloc.allocate(2);
  std::cout << "Allocated address " << t << ", Allocated value " << *t << std::endl;
  t[0] = 1;
  alloc.deallocate(t, 2);
  EXPECT_TRUE(alloc.num_pools == 1);
}

/*
TEST(MyAllocatorTest, DeleteAllocatorAssertions) {
  std::shared_ptr<int[]> tmp;
  auto t = std::make_shared<int*>(new int[10]);
  std::cout << t.get()[0] << std::endl;
  EXPECT_TRUE(0 == 0);
}
*/

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