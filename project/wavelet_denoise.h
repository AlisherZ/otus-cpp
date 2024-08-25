#ifndef WAVELET_DENOISE_H_
#define WAVELET_DENOISE_H_

void wavelet_denoise(double *inp, double *out, int N, int J, const char *wname, const char *dmethod, const char *method, const char *ext, const char *thresh, const char *level);

#endif // WAVELET_DENOISE_H_
