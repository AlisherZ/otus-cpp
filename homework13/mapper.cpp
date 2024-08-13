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
    //auto clf = TfClassifier{"train/saved_model", width, height};
    auto clf = TfClassifier{"saved_model", width, height};

    auto proba_true = TfClassifier::probas_t{};
    size_t class_true;
    auto features = TfClassifier::features_t{};
    auto features1 = TfClassifier::features_t{};
    //std::ifstream test_data{"train/test_mnist_cnn.txt"};
    std::ifstream test_data{"test_data_cnn.txt"};
    for (;;) {
        proba_true.clear();
        for (size_t i = 0; i < 1; ++i) {
            //float val;
            //test_data >> val;
            //proba_true.push_back(val);
            test_data >> class_true;
        }
        if (!read_features(test_data, features)) {
            break;
        }
        features1 = features;
        auto proba_pred = clf.predict_proba(features);
        //std::cout << proba_true[0] << " : ";
        std::cout << class_true << " : ";
        for (size_t i = 0; i < output_dim; ++i) {
            //std::cout << proba_true[i] << " ; " << proba_pred[i];
            std::cout << proba_pred[i] << " ; ";
        }
        auto class_pred = clf.predict_class(features);
        std::cout << class_pred << std::endl;
    }
}
