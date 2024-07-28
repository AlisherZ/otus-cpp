#include "find_duplicate.h"

void FinderDuplicate::addIncludedDirectories(boost::filesystem::path path) {
    included_directories.push_back(path);
}

void FinderDuplicate::addIncludedDirectories(list_paths paths) {
    included_directories.insert(included_directories.end(), paths.begin(), paths.end());
}

void FinderDuplicate::addExcludedDirectories(boost::filesystem::path path) {
    excluded_directories.push_back(path);
}

void FinderDuplicate::addExcludedDirectories(list_paths paths) {
    excluded_directories.insert(excluded_directories.end(), paths.begin(), paths.end());
}

void FinderDuplicate::setHash(HashType new_type) {
    type = new_type;
}

void FinderDuplicate::setBlockSize(std::size_t new_block_size) {
    block_size = new_block_size;
}

void FinderDuplicate::setLevel(std::size_t new_level) {
    level = new_level;
}

void FinderDuplicate::setMinSize(std::size_t new_min_size) {
    min_size = new_min_size;
}

std::vector<list_paths> FinderDuplicate::findDuplicates() {
    set_paths excluded_directories1(excluded_directories.begin(), excluded_directories.end());
    auto paths = getFiles(included_directories, excluded_directories1, level, min_size);
    auto files = toFileReader(paths);
    auto grouppedFiles = groupByContent(files);
    auto duplicateFiles = filterUniqueFiles(grouppedFiles);
    std::vector<list_paths> result;
    for(auto duplicates : duplicateFiles) {
        result.push_back(toPaths(duplicates));
    }
    return result;
}

std::vector<FileReader> FinderDuplicate::toFileReader(list_paths paths) {
    std::vector<FileReader> result;
    for(auto path : paths) {
        result.emplace_back(path, block_size, type);
    }
    return result;
}

list_paths FinderDuplicate::toPaths(std::vector<FileReader> files) {
    list_paths result;
    for(auto& file : files) {
        result.push_back(file.getPath());
    }
    return result;
}

std::vector<std::vector<FileReader> > FinderDuplicate::filterUniqueFiles(std::vector<std::vector<FileReader> > files) {
    std::vector<std::vector<FileReader> > result;
    for(auto duplicates : files) {
        if(duplicates.size() > 1) {
            result.push_back(duplicates);
        }
    }
    return result;
}
