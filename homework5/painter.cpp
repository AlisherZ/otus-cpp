#include <cstdlib>
#include <iostream>
#include <string>
#include "pen.h"
#include "shapes.h"
#include "image.h"
#include "controller.h"

int main(int, char **)
{
    Controller app;

    app.openImage();
    app.newImage();
    app.openImage("C:\\file1.png");
    app.addShape<shape_type::point>(1, 2);
    app.changePen(std::make_tuple<uint8_t, uint8_t, uint8_t>(128, 128, 128));
    app.changePen(color::cyan, true);
    app.changePen((std::uint16_t)10);
    app.changePen(line_type::dotted);
    app.changePen(true);
    app.addShape<shape_type::line>(0, 0, 10, 20);
    app.saveImage("C:\\file2.png");
    app.clear();

    return 0;
}
