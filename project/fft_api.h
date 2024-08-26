#pragma once

#include <vector>

#include "filter.pb.h"

void fft_denoise_cpp(std::vector<double> inp, std::vector<double> out, FFT fft);
