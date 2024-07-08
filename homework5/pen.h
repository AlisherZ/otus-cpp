#pragma once

#include <cstdint>
#include <tuple>
#include <string>
#include "util.hpp"

enum class color
{
    black = 0x000000,
    red = 0xFF0000,
    green = 0x00FF00,
    blue = 0x0000FF,
    cyan = 0x00FFFF,
    magenta = 0xFF00FF,
    yellow = 0xFFFF00,
    white = 0xFFFFFF
};

enum class line_type
{
    solid,
    dashed,
    dotted
};

class Pen
{
public:
    Pen() {};
    Pen(Pen &pen) : lineColor(pen.lineColor), thickness(pen.thickness), type(pen.type), isFill(pen.isFill), fillColor(pen.fillColor){};
    ColorRGB getLineColor();
    void setLineColor(std::uint32_t lineColor);
    void setLineColor(ColorRGB color);
    void setLineColor(color color);
    std::int16_t getThickness();
    void setThickness(std::uint16_t thickness);
    line_type getType();
    void setType(line_type type);
    bool getIsFill();
    void setIsFill(bool isFill);
    ColorRGB getFillColor();
    void setFillColor(std::int32_t fillColor);
    void setFillColor(ColorRGB color);
    void setFillColor(color color);
    std::string toString();
private:
    std::uint32_t lineColor = 0;
    std::uint16_t thickness = 1;
    line_type type = line_type::solid;
    bool isFill = false;
    std::int32_t fillColor = 0;
    ColorRGB getColorRGB(std::uint32_t color);
    std::string getColorString(std::uint32_t color);
};

std::string LineTypeString(line_type type);
