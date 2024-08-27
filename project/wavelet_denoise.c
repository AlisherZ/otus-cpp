#include <stdlib.h>
#include "wavelet_denoise.h"
#include "header/wauxlib.h"

void wavelet_denoise(double *inp, double *out, int N, int J, const char *wname, const char *dmethod, const char *method, const char *ext, const char *thresh, const char *level) {
	denoise_object obj = denoise_init(N, J, wname);
	setDenoiseMethod(obj, dmethod);
	setDenoiseWTMethod(obj, method);
	setDenoiseWTExtension(obj, ext);
	setDenoiseParameters(obj, thresh, level);
	denoise(obj, inp, out);
}
