//
// Created by bugra on 16.06.2024.
//

#ifndef HTTP_SERVER_STARTER_CPP_HTTPMESSAGEHANDLER_H
#define HTTP_SERVER_STARTER_CPP_HTTPMESSAGEHANDLER_H

//TODO: Instead of making all of them static try to find a better way with interfaces.
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
#include <fstream>
#include <iostream>
using string_vector = std::vector<std::string>;
class HTTPMessageHandler {
public:
    static const std::string HTTP_SUCCESS;
    static const std::string HTTP_NOT_FOUND;
    static const std::string APP_CONTENT;
    static const std::string TEXT_CONTENT;
    static const std::string CREATED;
    static std::string directory;

    HTTPMessageHandler() = default;
    static string_vector splitMessageIntoTokens(const std::string &message,const std::string& delim);

    static void handleResponseType(int socket_fd,const string_vector& tokens);
private:
    static string_vector validEncodings;
     static int sendMessageRespondToSocket(int socket, std::string buffer);
     static void handleErrorCommand(int socket_fd);
     static void handleSuccesCommand(int socket_fd);
     static void handleCreatedCommand(int socket_fd);
     static void handleEchoCommand(int socket_fd,string_vector &tokens);
     static void handleUserAgentCommand(int socket_fd,string_vector &tokens);
     static void handleFileCommand(int socket_fd,string_vector &tokens);
    static std::string convertStringIntoResponse(std::string &msg,std::string contentType,
                                                 std::string httpStatus,bool acceptEncoding,std::string encoding);
    static void handleFileWriteCommand(int socket_fd, std::string &filename,std::string &msg);
    static std::string getValidEncoding(const string_vector &encodings);
};


#endif //HTTP_SERVER_STARTER_CPP_HTTPMESSAGEHANDLER_H
