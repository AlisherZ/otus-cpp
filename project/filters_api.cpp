#include "filters_api.h"
#include "wavelet_cpp_api.h"
#include "fft_api.h"

#include <iostream>

std::vector<double> denoise_cpp(std::vector<double> inp, Filters filter) {
    std::vector<double> out(inp.size(), 0);
    std::cout << filter.filters_size() << std::endl;
    for(int i = 0;i < filter.filters_size();i++) {
        denoise_cpp(inp, out, filter.filters(i));
        inp = out;
    }
    return inp;
}

void denoise_cpp(std::vector<double> inp, std::vector<double> &out, Filter filter) {
    if(filter.has_wavelet()) {
        wavelet_denoise_cpp(inp, out, filter.wavelet());
    }
    if(filter.has_fft()) {
        fft_denoise_cpp(inp, out, filter.fft());
    }
}
