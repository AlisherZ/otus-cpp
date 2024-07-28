#include <boost/algorithm/hex.hpp>
#include <boost/compute/detail/sha1.hpp>
#include <boost/crc.hpp>
#include <boost/uuid/detail/md5.hpp>
#include "hash.h"

std::string toString(const boost::uuids::detail::md5::digest_type &digest)
{
    const auto charDigest = reinterpret_cast<const char *>(&digest);
    std::string result;
    boost::algorithm::hex(charDigest, charDigest + sizeof(boost::uuids::detail::md5::digest_type), std::back_inserter(result));
    return result;
}

std::string getHashCRC32(std::string data) {
    boost::crc_32_type result;
    result.process_bytes(data.data(), data.length());
    return std::to_string(result.checksum());
}

std::string getHashMD5(std::string data) {
    boost::uuids::detail::md5 hash;
    boost::uuids::detail::md5::digest_type digest;

    hash.process_bytes(data.data(), data.size());
    hash.get_digest(digest);

    return toString(digest);
}

std::string getHashSHA1(std::string data) {
    boost::compute::detail::sha1 sha1 { data };
    std::string result { sha1 };
    return result;
}

std::string getHashNone(std::string data) {
    return data;
}

std::string getHash(std::string data, HashType type) {
    switch(type) {
        case HashType::CRC32: return getHashCRC32(data);
        case HashType::MD5: return getHashMD5(data);
        case HashType::SHA1: return getHashSHA1(data);
        case HashType::None: return getHashNone(data);
    }
    throw std::invalid_argument( "received non-existent type of hash" );
}

HashType getHashType(std::string type) {
    if(type == "crc32") {
        return HashType::CRC32;
    }
    if(type == "md5") {
        return HashType::MD5;
    }
    if(type == "sha1") {
        return HashType::SHA1;
    }
    if(type == "none") {
        return HashType::None;
    }
    throw std::invalid_argument( "received non-existent type of hash" );
}
