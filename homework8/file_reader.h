#pragma once

#include <boost/filesystem.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include <cstdint>
#include <memory>

#include "hash.h"

class FileReader {
public:
    FileReader(boost::filesystem::path ph1, std::size_t block_size1, HashType type1 = HashType::None) : ph(ph1), block_size(block_size1), type(type1), offset(0) {
        const boost::interprocess::mode_t mode = boost::interprocess::read_only;
        boost::interprocess::file_mapping fm(ph1.c_str(), mode);
        file_size = boost::filesystem::file_size(ph1);
        region = std::make_shared<boost::interprocess::mapped_region>(fm, mode, 0, 0);
        begin = static_cast<const char*>(region->get_address());
    };
    std::string readBlock();
    bool isFinished();
    boost::filesystem::path getPath();
protected:
    boost::filesystem::path ph;
    std::size_t offset;
    std::size_t block_size;
    std::size_t file_size;
    HashType type;
    std::shared_ptr<boost::interprocess::mapped_region> region;
    const char* begin;
};

std::vector<std::vector<FileReader> > groupByContent(std::vector<FileReader> files);
