#pragma once

#include "filters_api.h"
#include "wavelet_cpp_api.h"
#include "fft_api.h"

std::vector<double> fft_denoise_cpp(std::vector<double> inp, Filters filter) {
    for(int i = 0;i < filter.filters_size();i++) {
        std::vector<double> out;
        fft_denoise_cpp(inp, out, filter.filters(i));
        inp = out;
    }
    return inp;
}

void fft_denoise_cpp(std::vector<double> inp, std::vector<double> out, Filter filter) {
    if(filter.has_wavelet()) {
        wavelet_denoise_cpp(inp, out, filter.wavelet());
    }
    if(filter.has_fft()) {
        fft_denoise_cpp(inp, out, filter.fft());
    }
}
