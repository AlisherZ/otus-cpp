#include <fftw.h>

#include "fft.h"

void fft_denoise_cpp(std::vector<double> inp, std::vector<double> out, FFT fft) {
    fftw_complex in[inp.size()], oup[out.size()];
    fftw_plan p = fftw_create_plan(inp.size(), FFTW_FORWARD, FFTW_ESTIMATE);
    for(int i = 0;i < inp.size();i++) {
        in[i] = inp[i];
    }
    fftw_one(p, in, oup);
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
    p = fftw_create_plan(inp.size(), FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_one(p, oup, in);
    fftw_destroy_plan(p);
    for(int i = 0;i < inp.size();i++) {
        out[i] = in[i];
    }
}
