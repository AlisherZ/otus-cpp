#include <map>

#include "file_reader.h"

std::string FileReader::readBlock() {
    std::string str_data;
    str_data.assign(begin + offset, begin + std::min(offset + block_size, file_size));
    if(block_size != str_data.size()) {
        str_data+= std::string(block_size - str_data.size(), '\0');
    }
    offset+= block_size;
    return getHash(str_data, type);
}

bool FileReader::isFinished() {
    return (offset >= file_size);
}

boost::filesystem::path FileReader::getPath() {
    return ph;
}

std::vector<std::vector<FileReader> > groupByContent(std::vector<FileReader> files) {
    std::vector<std::vector<FileReader> > result;
    std::vector<FileReader> current;
    std::map<std::string, std::vector<FileReader> > files1;
    for(auto& file : files) {
        if(file.isFinished()) {
            current.push_back(file);
        }
        else {
            files1[file.readBlock()].push_back(file);
        }
    }
    for(auto it : files1) {
        auto groups = groupByContent(it.second);
        result.insert(result.end(), groups.begin(), groups.end());
    }
    result.push_back(current);
    return result;
}
