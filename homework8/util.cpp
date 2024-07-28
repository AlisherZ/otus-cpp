#pragma once

#include "util.h"

list_paths getFiles(list_paths dir_include, set_paths dir_exclude, std::size_t level, std::size_t min_size) {
    list_paths result;
    for(auto dir_start : dir_include) {
        boost::filesystem::recursive_directory_iterator dir(dir_start), end;
        while (dir != end) {
            if((is_regular_file(dir->path())) && (boost::filesystem::file_size(dir->path()) > min_size)) {
                result.push_back(dir->path());
            }
            if ((dir.depth() == level) || (dir_exclude.find(dir->path()) != dir_exclude.end())) {
                dir.disable_recursion_pending();
            }
            ++dir;
        }
    }
    return result;
}

list_paths getPaths(std::vector<std::string> paths) {
    list_paths result;
    for(auto path : paths) {
        result.emplace_back(path);
    }
    return result;
}
