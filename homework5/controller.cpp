#include <iostream>
#include <vector>
#include "controller.h"

std::string getFilePath() {
    return "C:\\file.png";
}

void Controller::newImage() {
    std::cout << "New image" << std::endl;
    img = std::make_unique<ImageDocument>(ImageDocument());
    clear();
}

void Controller::openImage() {
    std::string filePath = getFilePath();
    openImage(filePath);
}

void Controller::openImage(std::string fileName) {
    img->readImage(fileName);
}

void Controller::saveImage() {
    std::string filePath = getFilePath();
    saveImage(filePath);
}

void Controller::saveImage(std::string fileName) {
    img->writeImage(fileName);
}

void Controller::changePen(ColorRGB color, bool filled) {
    if(filled) {
        pen->setFillColor(color);
    }
    else {
        pen->setLineColor(color);
    }
}

void Controller::changePen(std::uint32_t color, bool filled) {
    if(filled) {
        pen->setFillColor(color);
    }
    else {
        pen->setLineColor(color);
    }
}

void Controller::changePen(color color, bool filled) {
    if(filled) {
        pen->setFillColor(color);
    }
    else {
        pen->setLineColor(color);
    }
}

void Controller::changePen(std::uint16_t thickness) {
    pen->setThickness(thickness);
}

void Controller::changePen(line_type type) {
    pen->setType(type);
}

void Controller::changePen(bool isFill) {
    pen->setIsFill(isFill);
}

void Controller::changePen(Pen &newPen) {
    Pen t(newPen);
    pen = std::make_shared<Pen>(t);
}

void Controller::deleteShape(int i) {
    shapes.erase(shapes.begin() + i);
    invalidate();
}

void Controller::clear() {
    shapes.clear();
    invalidate();
}

void Controller::addShape(std::unique_ptr<Shape> s) {
    shapes.push_back(std::move(s));
    invalidate();
}

void Controller::invalidate() {
     img->clear();
     GraphicalContext gc = img->getGraphicalContext();
     for(auto it = shapes.begin();it != shapes.end();it++) {
        (*it)->draw(gc, pen);
     }
}
