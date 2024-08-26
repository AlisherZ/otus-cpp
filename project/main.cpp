#include <boost/program_options.hpp>
#include <sciplot/sciplot.hpp>
#include <iostream>
#include <fstream>
#include "filter.pb.h"
#include "filters_api.h"
#include "output.h"

using namespace std;
namespace po = boost::program_options;

int main(int argc, char* argv[])
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("input_file,i", po::value<std::string>(), "input signal file.")
        ("config,c", po::value<std::string>(), "filters config file.")
        ("output,o", po::value<std::vector<std::string> >(), "Output types(txt|svg).")
        ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
 
    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 0;
    }

    if (vm.count("input_file") == 0) {
        std::cout << "Must select input signal." << std::endl;
        return 1;
    }

    if (vm.count("output") == 0) {
        std::cout << "Must select at least 1 output format." << std::endl;
        return 1;
    }

    Filters filt;
    /*
    auto f = filt.add_filters();
    auto ft = f->mutable_fft();
    ft->set_name("HighPass");
    ft->set_low_threshold(4);
    std::string current;
    filt.SerializeToString(&current);
    for(int i = 0;i < (int)current.size();i++) {
        std::cout << (int)current[i] << std::endl;
    }
    */

    if (vm.count("config")) {
        std::ifstream input_config(vm["config"].as<std::string>());
        filt.ParseFromIstream(&input_config);
        input_config.close();
    }

    std::vector<double> inp;

    std::ifstream input(vm["input_file"].as<std::string>());
    std::string line;
    while (std::getline(input, line)) {
        inp.push_back(std::stod(line));
    }
    input.close();

    auto out = denoise_cpp(inp, filt);

    for(auto format : vm["output"].as<std::vector<std::string> >()) {
        if(format == "txt") {
            std::ofstream output("result.txt");
            for(auto val : out) {
                output << val << std::endl;
            }
            output.close();
        }
    }

    return 0;
}
