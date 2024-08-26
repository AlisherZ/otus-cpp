#pragma once

#include "filter.pb.h"

std::vector<double> denoise_cpp(std::vector<double> inp, Filters filter);
void denoise_cpp(std::vector<double> inp, std::vector<double> &out, Filter filter);
