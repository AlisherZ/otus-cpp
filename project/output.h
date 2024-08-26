#pragma once

#include <string>
#include <vector>

void outputInAllFormats(std::vector<double> values, std::vector<std::string> formats);
void saveTxtFile(std::vector<double> values);
void saveImgFile(std::vector<double> values);
