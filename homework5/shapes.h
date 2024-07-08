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
    virtual void draw(GraphicalContext &gc, std::shared_ptr<Pen> &p) = 0;
    virtual std::string toString() = 0;
    virtual ~Shape(){};
};

class Point : public Shape
{
public:
    Point() {}; 
    Point(double x1, double y1) : x(x1), y(y1) {};
    Point(Point &p) : x(p.x), y(p.y) {};
    double get(std::size_t i);
    Point diff(Point &p);
    Point shift(Point &p);
    void draw(GraphicalContext &gc, std::shared_ptr<Pen> &p) override;
    std::string toString() override;
    Point operator-(Point p);
    Point operator+(Point p);
private:
    double x = 0;
    double y = 0;
};

class Line : public Shape
{
    public:
    Line(double x1, double y1, double x2, double y2) : first(Point(x1, y1)), second(Point(x2, y2)) {};
    Line(Point p1, Point p2) : first(p1), second(p2) {};
    Line(Line &line) : first(line.first), second(line.second) {};
    void draw(GraphicalContext &gc, std::shared_ptr<Pen> &p) override;
    std::string toString() override;
private:
    Point first;
    Point second;
};

class Rectangle : public Shape
{

    public:
    Rectangle(double x1, double y1, double x2, double y2) : corner(Point(x1, y1)), w(std::abs(x1 - x2)), h(std::abs(y1 - y2)) {};
    Rectangle(Point p1, double w1, double h1) : corner(p1), w(w1), h(h1) {};
    Rectangle(Point p1, Point p2) : corner(p1), w(std::abs((p1 - p2).get(0))), h(std::abs((p1 - p2).get(1))) {};
    Rectangle(Rectangle &rect) : corner(rect.corner), w(rect.w), h(rect.h) {};
    void draw(GraphicalContext &gc, std::shared_ptr<Pen> &p) override;
    std::string toString() override;
protected:
    Point corner;
private:
    double w;
    double h;
};

class Ellipse : public Rectangle
{

    public:
    Ellipse(double x1, double y1, double x2, double y2) : Rectangle(x1, y1, x2, y2), r1(std::abs(x1 - x2) / 2), r2(std::abs(y1 - y2) / 2) {};
    Ellipse(Point p1, double r_1, double r_2) : Rectangle(p1, 2 * r_1, 2 * r_2), r1(r_1), r2(r_2) {};
    Ellipse(Point p1, Point p2) : Rectangle(p1, p2), r1(std::abs((p1 - p2).get(0)) / 2), r2(std::abs((p1 - p2).get(1)) / 2) {};
    Ellipse(Ellipse &line) : Rectangle(line), r1(line.r1), r2(line.r2) {};
    void draw(GraphicalContext &gc, std::shared_ptr<Pen> &p) override;
    std::string toString() override;
private:
    double r1;
    double r2;
};

template<shape_type type, typename ...Args>
std::unique_ptr<Shape> make_shape(Args... data) {
    if constexpr(type == shape_type::point) {
        return std::unique_ptr<Point>(new Point(data...));
    }
    if constexpr(type == shape_type::line) {
        return std::unique_ptr<Line>(new Line(data...));
    }
    if constexpr(type == shape_type::rectangle) {
        return std::unique_ptr<Rectangle>(new Rectangle(data...));
    }
    if constexpr(type == shape_type::ellipse) {
        return std::unique_ptr<Ellipse>(new Ellipse(data...));
    }
}
