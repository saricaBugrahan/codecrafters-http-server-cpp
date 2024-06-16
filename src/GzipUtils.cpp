//
// Created by bugra on 16.06.2024.
//

#include "../include/GzipUtils.h"
#include <zlib.h>
#include <stdexcept>
#include <cstring>
#include <sstream>
#include <iomanip>


//TODO: Search and try to understand the logic.
std::string gzip_compress(const std::string &data) {

    z_stream zs;

    memset(&zs, 0, sizeof(zs));

    if (deflateInit2(&zs, Z_BEST_COMPRESSION, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) {

        throw std::runtime_error("deflateInit2 failed while compressing.");

    }

    zs.next_in = (Bytef *)data.data();

    zs.avail_in = data.size();

    int ret;

    char outbuffer[32768];

    std::string outstring;

    do {

        zs.next_out = reinterpret_cast<Bytef *>(outbuffer);

        zs.avail_out = sizeof(outbuffer);

        ret = deflate(&zs, Z_FINISH);

        if (outstring.size() < zs.total_out) {

            outstring.append(outbuffer, zs.total_out - outstring.size());

        }

    } while (ret == Z_OK);

    deflateEnd(&zs);

    if (ret != Z_STREAM_END) {

        throw std::runtime_error("Exception during zlib compression: (" + std::to_string(ret) + ") " + zs.msg);

    }

    return outstring;

}
std::string toHexString(const std::string& data) {
    std::ostringstream oss;
    for (unsigned char ch : data) {
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)ch;
    }
    return oss.str();
}