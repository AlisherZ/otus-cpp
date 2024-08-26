#pragma once

#include <vector>

#include "filter.pb.h"

void wavelet_denoise_cpp(std::vector<double> inp, std::vector<double> out, int N, Wavelet wt);

#endif // WAVELET_DENOISE_H_
