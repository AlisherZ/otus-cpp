#include "util.hpp"

class ImageDocument
{
public:
    ImageDocument() : gc("Form1") {};
    GraphicalContext getGraphicalContext();
    void readImage(std::string filePath);
    void writeImage(std::string filePath);
private:
    GraphicalContext gc;
};
