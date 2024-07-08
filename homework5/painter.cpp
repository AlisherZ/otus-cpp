#include <cstdlib>
#include <iostream>
#include <string>
#include "pen.h"
#include "shapes.h"

int main(int, char **)
{
    std::shared_ptr<GraphicalContext> gc = std::make_shared<std::string>("Form1");

    Pen pen;
    std::cout << pen.toString() << std::endl;
    pen.setLineColor(std::make_tuple<uint8_t, uint8_t, uint8_t>(128, 128, 128));
    pen.setThickness(10);
    pen.setType(line_type::dotted);
    pen.setIsFill(true);
    pen.setFillColor(color::cyan);
    Pen pen1(pen);
    std::cout << pen.toString() << std::endl;

    auto sh = make_shape<shape_type::point>(0.0, 1.0);
    auto sh1 = make_shape<shape_type::point>(-2, 3);
    auto sh2 = sh + sh1;
    std::cout << sh.toString() << std::endl;
    std::cout << sh1.toString() << std::endl;
    std::cout << sh2.toString() << std::endl;
    sh2.draw(gc, pen);

    auto sh3 = make_shape<shape_type::line>(sh, sh2);
    auto sh4 = make_shape<shape_type::rectangle>(sh, sh2);
    auto sh5 = make_shape<shape_type::ellipse>(sh, sh2);
    sh3.draw(gc, pen);
    sh4.draw(gc, pen);
    sh5.draw(gc, pen);

    return 0;
}
