#include <stdlib.h>
#include "wavelet_denoise.h"
#include "header/wauxlib.h"

void wavelet_denoise(double *inp, double *out, int N, int J, const char *wname, const char *dmethod, const char *method, const char *ext, const char *thresh, const char *level) {
    //struct denoise_set t;
    //double *sig, *inp, *oup;
    //char *wname = "db5";
	//char *method = "dwt";
	//char *ext = "sym";
	//char *thresh = "soft";
	//char *level = "first";
	//char *denoise_method = "minimax";
    //N = 951;
	//J = 4;
	//inp = (double*)malloc(sizeof(double)* N);
	//oup = (double*)malloc(sizeof(double)* N);
    //visushrink(inp, N, J, wname, method, ext, thresh, level, oup);
	denoise_object obj = denoise_init(N, J, wname);
	setDenoiseMethod(obj, dmethod);
	setDenoiseWTMethod(obj, method);
	setDenoiseWTExtension(obj, ext);
	setDenoiseParameters(obj, thresh, level);
	denoise(obj, inp, out);
}
