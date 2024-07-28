#pragma once

#include "file_reader.h"
#include "hash.h"
#include "util.h"

class FinderDuplicate {
public:
    FinderDuplicate() : type(HashType::None), block_size(1), level(0), min_size(1) {};
    void addIncludedDirectories(boost::filesystem::path path);
    void addIncludedDirectories(list_paths paths);
    void addExcludedDirectories(boost::filesystem::path path);
    void addExcludedDirectories(list_paths paths);
    void setHash(HashType new_type);
    void setBlockSize(std::size_t new_block_size);
    void setLevel(std::size_t new_level);
    void setMinSize(std::size_t new_min_size);
    std::vector<list_paths> findDuplicates();
private:
    std::vector<FileReader> toFileReader(list_paths paths);
    list_paths toPaths(std::vector<FileReader> files);
    std::vector<std::vector<FileReader> > filterUniqueFiles(std::vector<std::vector<FileReader> > files);
    list_paths included_directories;
    list_paths excluded_directories;
    std::string regex_mask;
    HashType type;
    std::size_t block_size;
    std::size_t level;
    std::size_t min_size;
};
