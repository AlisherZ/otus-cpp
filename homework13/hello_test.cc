#include <stdio.h>
#include <fstream>

#include "helpers.h"
#include "tf_classifier.h"

#include <gtest/gtest.h>

const size_t width = 28;
const size_t height = 28;
const size_t output_dim = 10;

TEST(HelloTest, BasicAssertions) {
  auto clf = mnist::TfClassifier{"saved_model", width, height};

  size_t class_true;
  auto features = mnist::TfClassifier::features_t{};
  //std::ifstream test_data{"train/test_mnist_cnn.txt"};
  std::ifstream test_data{"test_data_cnn.txt"};
  for (;;) {
      test_data >> class_true;
      if (!mnist::read_features(test_data, features)) {
          break;
      }
      auto class_pred = clf.predict_class(features);
      EXPECT_EQ(class_true, class_pred);
  }
}
