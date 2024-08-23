#include <iostream>
#include <fstream>
#include <complex.h>
//#include <fftw3.h>
#include "message.pb.h"
#include "wavelet_denoise.h"

using namespace std;

int main(int, char* argv[])
{
    Person john;
    john.set_id(1234);
    john.set_name("John Doe");
    john.set_email("jdoe@example.com");
    
    std::ofstream output(argv[0]);
    john.SerializeToOstream(&output);
    output.close();
    //const int N = 10;
    
    /*
    auto obj = denoise_init(N,4,"db5");
	setDenoiseMethod(obj,"visushrink");// sureshrink is also the default. The other option with dwt and swt is visushrink.
	// modwt works only with modwtshrink method
	setDenoiseWTMethod(obj,"dwt");// Default is dwt. the other options are swt and modwt
	setDenoiseWTExtension(obj,"sym");// Default is sym. the other option is per
	setDenoiseParameters(obj,"soft","all");// Default for thresh is soft. Other option is hard
	// Default for level is all. The other option is first

	//denoise(obj,inp,oup);
    */
    return 0;
}
