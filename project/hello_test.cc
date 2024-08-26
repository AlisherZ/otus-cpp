#include <gtest/gtest.h>
#include <iostream>
#include <fstream>

#include "filters_api.h"

// Demonstrate some basic assertions.
TEST(FiltersTest, EmptyAssertions) {
  Filters filt;
  
  std::vector<double> inp;
  std::string line;

  std::ifstream input("test_input.txt");
  while (std::getline(input, line)) {
    inp.push_back(std::stod(line));
  }
  input.close();

  std::vector<double> out;
  std::ifstream input1("test_output.txt");
  while (std::getline(input1, line)) {
    out.push_back(std::stod(line));
  }
  input1.close();

  auto out1 = fft_denoise_cpp(inp, filt);

  EXPECT_EQ(out.size(), out1.size());
}
