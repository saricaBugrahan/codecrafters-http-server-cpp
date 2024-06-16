//
// Created by bugra on 16.06.2024.
//

#ifndef HTTP_SERVER_STARTER_CPP_CONNECTIONHANDLER_H
#define HTTP_SERVER_STARTER_CPP_CONNECTIONHANDLER_H


enum MSG{OK=0,ERROR=1};
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include "../include/HTTPMessageHandler.h"
class ConnectionHandler {

public:
    int server_fd;
    ConnectionHandler() = default;
    int createServerSocket();
    int setServerSocketForReuse() const;
    int bindServerSocketToPort(int port) const;
    int startListeningThePort() const;
    void handleClient() const;
    std::string getMessageFromSocket(int client_fd) const;
};


#endif //HTTP_SERVER_STARTER_CPP_CONNECTIONHANDLER_H
