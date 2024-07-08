#pragma once

#include <memory>
#include "util.hpp"

class ImageDocument
{
public:
    ImageDocument() : gc("Form1"), data(std::make_unique<ColorRGB[]>(256)) {};
    GraphicalContext getGraphicalContext();
    void readImage(std::string filePath);
    void writeImage(std::string filePath);
    void clear();
private:
    GraphicalContext gc;
    std::unique_ptr<ColorRGB[]> data;
};
