#include <cstdint>
#include <stdexcept>
#include <tuple>
#include "pen.h"
#include "util.hpp"

std::string LineTypeString(line_type type)
{
    switch (type)
    {
        case line_type::solid : return "solid" ;
        case line_type::dashed : return "dashed";
        case line_type::dotted : return "dotted";
    };
    throw std::invalid_argument( "received non-existent type of line" );
}

ColorRGB Pen::getColorRGB(std::uint32_t color) {
    const int COLOR_BYTES = 3;
    std::uint8_t colorBytes[COLOR_BYTES];
    for(int i = 0;i < COLOR_BYTES;i++) {
        colorBytes[COLOR_BYTES - i - 1] = (color >> (8 * i)) % 256;
    }
    return std::make_tuple(colorBytes[0], colorBytes[1], colorBytes[2]);
}


std::string Pen::getColorString(std::uint32_t color) {
    const int COLOR_BYTES = 3;
    std::string res = "(";
    for(int i = 0;i < COLOR_BYTES;i++) {
        res+= (i > 0 ? ", ": "") + std::to_string((color >> (8 * i)) % 256);
    }
    res+= ")";
    return res;
}

ColorRGB Pen::getLineColor() {
    return getColorRGB(lineColor);
}

std::int16_t Pen::getThickness() {
    return thickness;
}

line_type Pen::getType() {
    return type;
}

bool Pen::getIsFill() {
    return isFill;
}

ColorRGB Pen::getFillColor() {
    return getColorRGB(fillColor);
}

void Pen::setLineColor(std::uint32_t newLineColor) {
    lineColor = newLineColor;
}

void Pen::setLineColor(ColorRGB color) {
    lineColor = sum_elements(color);
}

void Pen::setLineColor(color color) {
    lineColor = static_cast<std::uint32_t>(color);
}

void Pen::setThickness(std::uint16_t newThickness) {
    thickness = newThickness;
}

void Pen::setType(line_type newType) {
    type = newType;
}

void Pen::setIsFill(bool newIsFill) {
    isFill = newIsFill;
}

void Pen::setFillColor(std::int32_t newFillColor) {
    fillColor = newFillColor;
}

void Pen::setFillColor(ColorRGB color) {
    fillColor = sum_elements(color);
}

void Pen::setFillColor(color color) {
    fillColor = static_cast<std::uint32_t>(color);
}

std::string Pen::toString() {
    std::string res = "(";
    res+= "Line color: " + getColorString(lineColor) + "; ";
    res+= "Line thickness: " + std::to_string(thickness) + "; ";
    res+= "Line type: " + LineTypeString(type) + "; ";
    res+= "Fill shape: " + std::to_string(isFill) + (isFill ? "; " : "");
    if(isFill) {
        res+= "Fill color: " + getColorString(fillColor);
    }
    res+= ")";
    return res;
}
