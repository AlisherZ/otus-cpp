#include <cstdlib>
#include <iostream>
#include <string>
#include "pen.h"

int main(int, char **)
{
    Pen pen;
    std::cout << pen.toString() << std::endl;
    pen.setLineColor(std::make_tuple<uint8_t, uint8_t, uint8_t>(128, 128, 128));
    pen.setThickness(10);
    pen.setType(line_type::dotted);
    pen.setIsFill(true);
    pen.setFillColor(color::cyan);
    Pen pen1(pen);
    std::cout << pen.toString() << std::endl;

    return 0;
}
