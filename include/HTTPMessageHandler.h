//
// Created by bugra on 16.06.2024.
//

#ifndef HTTP_SERVER_STARTER_CPP_HTTPMESSAGEHANDLER_H
#define HTTP_SERVER_STARTER_CPP_HTTPMESSAGEHANDLER_H


#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <vector>
#include <unistd.h>
using string_vector = std::vector<std::string>;

class HTTPMessageHandler {

public:
    const std::string HTTP_SUCCESS = "HTTP/1.1 200 OK\r\n\r\n";
    const std::string HTTP_NOT_FOUND = "HTTP/1.1 404 NOT FOUND";
    HTTPMessageHandler() = default;
    string_vector splitMessageIntoTokens(const std::string &message,std::string delim);
    static int sendMessageToSocket(int socket,std::string &buffer);
};


#endif //HTTP_SERVER_STARTER_CPP_HTTPMESSAGEHANDLER_H
