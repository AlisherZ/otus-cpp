#include "wavelet_cpp_api.h"
#include "wavelet_denoise.h"

void wavelet_denoise_cpp(std::vector<double> inp, std::vector<double> &out, Wavelet wt) {
    wavelet_denoise(inp.data(), out.data(), inp.size(), wt.levels_decomposition(), wt.name().c_str(), wt.denoising_method().c_str(), 
        wt.decomposition_method().c_str(), wt.signal_extension().c_str(), wt.thresholding().c_str(), wt.estimation_level().c_str());
}
