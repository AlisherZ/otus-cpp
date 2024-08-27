#include <boost/program_options.hpp>
#include <google/protobuf/util/json_util.h>
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
        ("output,o", po::value<std::vector<std::string> >(), "Output types(txt|svg|cout).")
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

    if (vm.count("config")) {
        std::ifstream input_config(vm["config"].as<std::string>());
        std::string config;
        input_config >> config;
        google::protobuf::util::JsonStringToMessage(config, &filt);
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

    outputInAllFormats(out, vm["output"].as<std::vector<std::string> >());

    return 0;
}
