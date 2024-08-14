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

int main(int argc, char* argv[]) {

    try {
        if (argc != 3) {
            std::cerr << "Usage: fashio_mnist <data> <model>\n";
            return 1;
        }

        auto clf = TfClassifier{argv[2], width, height};

        size_t class_true;
        size_t num_data = 0, num_correct = 0;
        auto features = TfClassifier::features_t{};
        auto features1 = TfClassifier::features_t{};
        std::ifstream test_data{argv[1]};
        for (;;) {
            test_data >> class_true;
            if (!read_features(test_data, features)) {
                break;
            }
            auto class_pred = clf.predict_class(features);
            num_data++;
            if(class_true == class_pred) {
                num_correct++;
            }
        }
        std::cout << num_correct * 1.0 / num_data << std::endl;
    }
    catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << "\n";
    }
}
