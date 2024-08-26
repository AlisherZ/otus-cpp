#include <sciplot/sciplot.hpp>

#include "output.h"

void outputInAllFormats(std::vector<double> values, std::vector<std::string> formats) {
    for(auto format : formats) {
        if(format == "txt") {
            saveTxtFile(val);
        }
        if(format == "svg") {
            saveImgFile(val);
        }
    }
}

void saveTxtFile(std::vector<double> values) {
    std::ofstream output("result.txt");
    for(auto val : values) {
        output << val << std::endl;
    }
    output.close();
}

void saveImgFile(std::vector<double> values) {

}
