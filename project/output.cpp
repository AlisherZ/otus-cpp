#include <sciplot/sciplot.hpp>

#include "output.h"

void outputInAllFormats(std::vector<double> values, std::vector<std::string> formats) {
    for(auto format : formats) {
        if(format == "txt") {
            saveTxtFile(values);
        }
        if(format == "svg") {
            saveImgFile(values);
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
    Vec x = linspace(0.0, values.size(), values.size());
    
    Plot2D plot;
    plot.xlabel("x");
    plot.ylabel("y");
    plot.xrange(0.0, values.size());
    plot.yrange(0.0, *max_element(values.begin(), values.end()) + 1);
    plot.legend()
        .atOutsideBottom()
        .displayHorizontal()
        .displayExpandWidthBy(2);
    plot.drawCurve(x, values[x]).label("signal");
    Figure fig = {{plot}};
    Canvas canvas = {{fig}};
    //canvas.show();
    canvas.save("result.svg");
}
