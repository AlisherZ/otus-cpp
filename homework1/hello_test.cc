#include "lib.h"

#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  EXPECT_GT(version(), 0);
}