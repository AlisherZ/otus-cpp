#include <stdlib.h>
#include "wavelet_denoise.h"
#include "wdenoise/header/wdenoise.h"

int test() {
    double *sig, *inp, *oup;
	int i, N, J;
    char *wname = "db5";
	char *method = "dwt";
	char *ext = "sym";
	char *thresh = "soft";
	char *level = "first";
	char *denoise_method = "minimax";
    N = 951;
	J = 4;
	inp = (double*)malloc(sizeof(double)* N);
	oup = (double*)malloc(sizeof(double)* N);
    visushrink(inp,N,J,wname,method,ext,thresh,level,oup);
    return 1;
}