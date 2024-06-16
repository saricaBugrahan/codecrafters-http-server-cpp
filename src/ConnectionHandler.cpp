//
// Created by bugra on 16.06.2024.
//
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include "ConnectionHandler.h"

int ConnectionHandler::createServerSocket() {
    server_fd = socket(AF_INET,SOCK_STREAM,0);
    if (server_fd < 0){
        std::cerr << "Failed to create server socket" << std::endl;
        return 1;
    }
    return server_fd;
}

int ConnectionHandler::setServerSocketForReuse() const {
    int reuse = 1;
    if (setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse))){
        std::cerr << "Failed to set REUSE" << std::endl;
        return 1;
    }
    return 0;
}

int ConnectionHandler::bindServerSocketToPort(int port) const {
    struct sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
        std::cerr << "Failed to bind to port 4221\n";
        return 1;
    }
    return 0;
}

int ConnectionHandler::startListeningThePort() {
    int connection_backlog = 5;
    if (listen(server_fd, connection_backlog) != 0) {
        std::cerr << "listen failed\n";
        return 1;
    }
    return 0;
}