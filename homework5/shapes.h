#pragma once

#include <memory>
#include "pen.h"
#include "util.hpp"

enum class shape_type
{
    point,
    line,
    ellipse,
    rectangle,
};

class Shape
{
public:
    virtual void draw(std::shared_ptr<GraphicalContext> gc, Pen p) = 0;
    virtual std::string toString() = 0;
};

class Point : Shape
{
public:
    Point() {}; 
    Point(double x1, double y1) : x(x1), y(y1) {};
    Point(Point &p) : x(p.x), y(p.y) {};
    double get(std::size_t i);
    Point diff(Point &p);
    Point shift(Point &p);
    void draw(std::shared_ptr<GraphicalContext> gc, Pen p) override;
    std::string toString() override;
    Point operator-(Point p);
    Point operator+(Point p);
private:
    double x = 0;
    double y = 0;
};

class Line : Shape
{
    public:
    Line(Point p1, Point p2) : first(p1), second(p2) {};
    Line(Line &line) : first(line.first), second(line.second) {};
    void draw(std::shared_ptr<GraphicalContext> gc, Pen p) override;
    std::string toString() override;
private:
    Point first;
    Point second;
};

class Rectangle : Shape
{

    public:
    Rectangle(Point p1, double w1, double h1) : corner(p1), w(w1), h(h1) {};
    Rectangle(Point p1, Point p2) : corner(p1), w(std::abs((p1 - p2).get(0))), h(std::abs((p1 - p2).get(1))) {};
    Rectangle(Rectangle &rect) : corner(rect.corner), w(rect.w), h(rect.h) {};
    void draw(std::shared_ptr<GraphicalContext> gc, Pen p) override;
    std::string toString() override;
protected:
    Point corner;
private:
    double w;
    double h;
};

class Ellipse : Rectangle
{

    public:
    Ellipse(Point p1, double r_1, double r_2) : Rectangle(p1, 2 * r_1, 2 * r_2), r1(r_1), r2(r_2) {};
    Ellipse(Point p1, Point p2) : Rectangle(p1, p2), r1(std::abs((p1 - p2).get(0)) / 2), r2(std::abs((p1 - p2).get(1)) / 2) {};
    Ellipse(Ellipse &line) : Rectangle(line), r1(line.r1), r2(line.r2) {};
    void draw(std::shared_ptr<GraphicalContext> gc, Pen p) override;
    std::string toString() override;
private:
    double r1;
    double r2;
};

template<shape_type type, typename ...Args>
auto make_shape(Args... data) {
    if constexpr(type == shape_type::point) {
        return Point(data...);
    }
    if constexpr(type == shape_type::line) {
        return Line(data...);
    }
    if constexpr(type == shape_type::rectangle) {
        return Rectangle(data...);
    }
    if constexpr(type == shape_type::ellipse) {
        return Ellipse(data...);
    }
}
