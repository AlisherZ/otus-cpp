#include <fftw3.h>
#include <memory>

#include "fft_api.h"

void fft_denoise_cpp(std::vector<double> inp, std::vector<double> &out, FFT fft) {
    fftw_complex *oup = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * out.size());
    std::shared_ptr<double[]> in(new double[inp.size()]);
    fftw_plan p = fftw_plan_dft_r2c_1d(inp.size(), inp.data(), oup, FFTW_ESTIMATE);
    fftw_execute(p);
    fftw_destroy_plan(p);
    if(fft.name() == "LowPass") {
        for(int i = fft.high_threshold() + 1;i < (int)inp.size();i++) {
            oup[i][0] = 0;
            oup[i][1] = 0;
        }
    }
    if(fft.name() == "HighPass") {
        for(int i = 0;i < fft.low_threshold();i++) {
            oup[i][0] = 0;
            oup[i][1] = 0;
        }
    }
    if(fft.name() == "BandPass") {
        for(int i = 0;i < fft.low_threshold();i++) {
            oup[i][0] = 0;
            oup[i][1] = 0;
        }
        for(int i = fft.high_threshold() + 1;i < (int)inp.size();i++) {
            oup[i][0] = 0;
            oup[i][1] = 0;
        }
    }
    if(fft.name() == "BandStop") {
        for(int i = fft.high_threshold() + 1;i < fft.low_threshold();i++) {
            oup[i][0] = 0;
            oup[i][1] = 0;
        }
    }
    p = fftw_plan_dft_c2r_1d(inp.size(), oup, in.get(), FFTW_ESTIMATE);
    fftw_execute(p);
    fftw_destroy_plan(p);
    fftw_free(oup);
    for(int i = 0;i < (int)inp.size();i++) {
        out[i] = in[i];
    }
}
