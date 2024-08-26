#include "wavelet_cpp_api.h"
#include "wavelet_denoise.h"

void wavelet_denoise_cpp(std::unique_ptr<double> inp, std::unique_ptr<double> out, int N, Wavelet wt) {
    wavelet_denoise(inp.data(), out1.data(), N, wt.get_levels_decomposition(), wt.get_name(), wt.get_denoising_method(), 
        wt.get_decomposition_method(), wt.get_signal_extension(), wt.get_thresholding(), wt.get_estimation_level());
}
