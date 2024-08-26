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
  for(std::size_t i = 0;i < out.size();i++) {
    EXPECT_DOUBLE_EQ(out[i], out1[i]);
  }
}

TEST(FiltersTest, FFTAssertions) {
  Filters filt;
  std::ifstream input_config("only_fft.txt");
  filt.ParseFromIstream(&input_config);
  input_config.close();
  
  std::vector<double> inp;
  std::string line;

  std::ifstream input("test_input.txt");
  while (std::getline(input, line)) {
    inp.push_back(std::stod(line));
  }
  input.close();

  std::vector<double> out;
  std::ifstream input1("test_fft_output.txt");
  while (std::getline(input1, line)) {
    out.push_back(std::stod(line));
  }
  input1.close();

  auto out1 = fft_denoise_cpp(inp, filt);

  EXPECT_EQ(out.size(), out1.size());
  for(std::size_t i = 0;i < out.size();i++) {
    EXPECT_NEAR(out[i], out1[i], 0.1);
  }
}

TEST(FiltersTest, WTAssertions) {
  Filters filt;
  std::ifstream input_config("only_wt.txt");
  filt.ParseFromIstream(&input_config);
  input_config.close();
  
  std::vector<double> inp;
  std::string line;

  std::ifstream input("test_input.txt");
  while (std::getline(input, line)) {
    inp.push_back(std::stod(line));
  }
  input.close();

  std::vector<double> out;
  std::ifstream input1("test_wt_output.txt");
  while (std::getline(input1, line)) {
    out.push_back(std::stod(line));
  }
  input1.close();

  auto out1 = fft_denoise_cpp(inp, filt);

  EXPECT_EQ(out.size(), out1.size());
  for(std::size_t i = 0;i < out.size();i++) {
    EXPECT_NEAR(out[i], out1[i], 0.1);
  }
}
