#pragma once

#include <boost/filesystem.hpp>

#include <string>
#include <vector>
#include <set>

using list_paths = std::vector<boost::filesystem::path>;
using set_paths = std::set<boost::filesystem::path>;

list_paths getFiles(list_paths dir_include, set_paths dir_exclude, std::size_t level, std::size_t min_size);

list_paths getPaths(std::vector<std::string> paths);
