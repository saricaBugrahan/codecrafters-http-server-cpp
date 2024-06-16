//
// Created by bugra on 16.06.2024.
//

#ifndef HTTP_SERVER_STARTER_CPP_GZIPUTILS_H
#define HTTP_SERVER_STARTER_CPP_GZIPUTILS_H


#include <string>

std::string gzip_compress(const std::string& data);
std::string toHexString(const std::string& data);

#endif //HTTP_SERVER_STARTER_CPP_GZIPUTILS_H
