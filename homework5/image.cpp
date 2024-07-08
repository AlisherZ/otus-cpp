#include <iostream>
#include "image.h"

GraphicalContext ImageDocument::getGraphicalContext() {
    return gc;
}

void ImageDocument::readImage(std::string filePath) {
    data = std::make_unique<ColorRGB[]>(256);
    gc = "File1";
    std::cout << "Read image " << filePath << std::endl;
}

void ImageDocument::writeImage(std::string filePath) {
    std::cout << "Write image " << filePath << std::endl;
}

void ImageDocument::clear() {
    gc = "File1";
    std::cout << "Clear painted part of image" << std::endl;
}
