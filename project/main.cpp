#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include "filter.pb.h"
#include "filters_api.h"

using namespace std;
namespace po = boost::program_options;

int main(int argc, char* argv[])
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("input_file,i", po::value<std::string>(), "input signal file")
        ("config,c", po::value<std::string>(), "filters config file. ")
        ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
 
    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 0;
    }

    if (vm.count("input_file") == 0) {
        std::cout << "Must select input signal" << std::endl;
        return 1;
    }

    Filters filt;

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

    auto out = fft_denoise_cpp(inp, filt);

    std::ofstream output("result.txt");
    for(auto val : out) {
        output << val << std::endl;
    }
    output.close();

    return 0;
}
