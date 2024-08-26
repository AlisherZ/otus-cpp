#include <fftw3.h>

#include "fft_api.h"

void fft_denoise_cpp(std::vector<double> inp, std::vector<double> out, FFT fft) {
    fftw_complex oup[out.size()];
    double in[inp.size()];
    fftw_plan p = fftw_plan_dft_r2c_1d(inp.size(), inp.data(), oup, FFTW_ESTIMATE);
    fftw_execute(p);
    fftw_destroy_plan(p);
    if(fft.get_name() == "LowPass") {
        for(int i = fft.get_high_threshold() + 1;i < inp.size();i++) {
            oup[i] = 0;
        }
    }
    if(fft.get_name() == "HighPass") {
        for(int i = 0;i < fft.get_low_threshold();i++) {
            oup[i] = 0;
        }
    }
    if(fft.get_name() == "BandPass") {
        for(int i = 0;i < fft.get_low_threshold();i++) {
            oup[i] = 0;
        }
        for(int i = fft.get_high_threshold() + 1;i < inp.size();i++) {
            oup[i] = 0;
        }
    }
    if(fft.get_name() == "BandStop") {
        for(int i = fft.get_high_threshold() + 1;i < fft.get_low_threshold();i++) {
            oup[i] = 0;
        }
    }
    p = fftw_plan_dft_c2r_1d(oup.size(), oup, in, FFTW_ESTIMATE);
    fftw_execute(p);
    fftw_destroy_plan(p);
    for(int i = 0;i < inp.size();i++) {
        out[i] = in[i];
    }
}
