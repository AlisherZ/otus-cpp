#include <iostream>
#include <stdexcept>
#include "shapes.h"

double Point::get(std::size_t i) {
    switch (i)
    {
        case 0 : return x;
        case 1 : return y;
    };
    throw std::invalid_argument( "received non-existent dimension" );
}

Point Point::diff(Point &p) {
    Point res(x - p.x, y - p.y);
    return res;
}

Point Point::shift(Point &p) {
    Point res(x + p.x, y + p.y);
    return res;
}

void Point::draw(GraphicalContext &gc, std::shared_ptr<Pen> &p) {
    std::cout << "Draw point: " << toString() << ", Graphical context: " << gc << ", selected pen: " << p->toString() << std::endl;
} 

std::string Point::toString() {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

Point Point::operator-(Point p) {
    return diff(p);
}

Point Point::operator+(Point p) {
    return shift(p);
}

void Line::draw(GraphicalContext &gc, std::shared_ptr<Pen> &p) {
    std::cout << "Draw line: " << toString() << ", Graphical context: " << gc << ", selected pen: " << p->toString() << std::endl;
}

std::string Line::toString() {
    return "Line: " + first.toString() + "-" + second.toString();
}

void Rectangle::draw(GraphicalContext &gc, std::shared_ptr<Pen> &p) {
    std::cout << "Draw rectangle: " << toString() << ", Graphical context: " << gc << ", selected pen: " << p->toString() << std::endl;
}

std::string Rectangle::toString() {
    return "Rectangle: " + corner.toString() + "->(+" + std::to_string(w) + ", +" + std::to_string(h) + ")";
}

void Ellipse::draw(GraphicalContext &gc, std::shared_ptr<Pen> &p) {
    std::cout << "Draw ellipse: " << toString() << ", Graphical context: " << gc << ", selected pen: " << p->toString() << std::endl;
}

std::string Ellipse::toString() {
    return "Ellipse: " + corner.toString() + "->(" + std::to_string(r1) + ", " + std::to_string(r2) + ")";
}
