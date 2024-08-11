#include <stdio.h>
#include <iostream>
#include <fstream>
#include <tensorflow/c/c_api.h>

#include "helpers.h"
#include "tf_classifier.h"

using namespace mnist;

const size_t width = 28;
const size_t height = 28;
const size_t output_dim = 10;

int main() {
    auto clf = TfClassifier{"train/saved_model", width, height};

    auto proba_true = TfClassifier::probas_t{};
    auto features = TfClassifier::features_t{};
    std::ifstream test_data{"train/test_mnist_cnn.txt"};
    for (;;) {
        proba_true.clear();
        for (size_t i = 0; i < output_dim; ++i) {
            float val;
            test_data >> val;
            proba_true.push_back(val);
        }
        if (!read_features(test_data, features)) {
            break;
        }
        auto proba_pred = clf.predict_proba(features);
        for (size_t i = 0; i < output_dim; ++i) {
            std::cout << proba_true[i] << " ; " <<  proba_pred[i] << std::endl;
        }
    }
}
