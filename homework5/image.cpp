#include <iostream>
#include "image.h"

GraphicalContext ImageDocument::getGraphicalContext() {
    return gc;
}

void ImageDocument::readImage(std::string filePath) {
    std::cout << "Read image " << filePath << std::endl;
}

void ImageDocument::writeImage(std::string filePath) {
    std::cout << "Write image " << filePath << std::endl;
}
