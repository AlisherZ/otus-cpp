#pragma once

#include "filter.pb.h"

std::vector<double> fft_denoise_cpp(std::vector<double> inp, Filters filter);
void fft_denoise_cpp(std::vector<double> inp, std::vector<double> out, Filter filter);
