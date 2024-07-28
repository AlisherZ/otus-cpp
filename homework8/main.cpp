#include <boost/program_options.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/compute/detail/sha1.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/crc.hpp>

#include <iostream>
#include <set>

#include "find_duplicate.h"

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("included_directories,i", po::value<std::vector<std::string> >(), "include directories for scan")
        ("excluded_directories,e", po::value<std::vector<std::string> >(), "exclude directories for scan")
        ("hash,H", po::value<std::string>(), "select hash function (md5|sha1|crc32|none)")
        ("block,b", po::value<std::size_t>(), "set size for block reading")
        ("level,l", po::value<std::size_t>(), "set level for scan")
        ("min_file,m", po::value<std::size_t>()->default_value(1), "set minimum size for file to scan")
        ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
 
    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 0;
    }

    if (vm.count("included_directories") == 0) {
        std::cout << "There are no directories to scan" << std::endl;
        return 1;
    }

    if (vm.count("hash") == 0) {
        std::cout << "Must select hash function" << std::endl;
        return 1;
    }

    if (vm.count("block") == 0) {
        std::cout << "Must set size of block" << std::endl;
        return 1;
    }

    if (vm.count("level") == 0) {
        std::cout << "Must set level of scan directories" << std::endl;
        return 1;
    }

    FinderDuplicate fd;
    fd.addIncludedDirectories(getPaths(vm["included_directories"].as<std::vector<std::string> >()));
    if (vm.count("excluded_directories")) {
        fd.addExcludedDirectories(getPaths(vm["excluded_directories"].as<std::vector<std::string> >()));
    }
    fd.setHash(getHashType(vm["hash"].as<std::string>()));
    fd.setBlockSize(vm["block"].as<std::size_t>());
    fd.setLevel(vm["level"].as<std::size_t>());
    fd.setMinSize(vm["min_file"].as<std::size_t>());

    auto res = fd.findDuplicates();
    std::cout << "Found duplicates:" << std::endl;
    for(auto duplicates : res) {
        for(auto path : duplicates) {
            std::cout << path.string() << std::endl;
        }
        std::cout << std::endl;
    }

    return 0;
}
