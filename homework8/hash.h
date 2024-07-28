#pragma once

#include <string>

enum class HashType {
    MD5,
    CRC32,
    SHA1,
    None
};

std::string getHash(std::string data, HashType type);

HashType getHashType(std::string type);
