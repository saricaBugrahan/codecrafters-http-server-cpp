//
// Created by bugra on 16.06.2024.
//

#include <cstring>
#include <atomic>
#include "../include/ConnectionHandler.h"

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

int ConnectionHandler::startListeningThePort() const {
    int connection_backlog = 5;
    if (listen(server_fd, connection_backlog) != 0) {
        std::cerr << "listen failed\n";
        return 1;
    }
    return 0;
}

std::string ConnectionHandler::getMessageFromSocket(int client_fd) const {
    std::string client_message(1024, '\0');

    ssize_t brecvd = recv(client_fd, (void *)&client_message[0], client_message.max_size(), 0);

    if (brecvd < 0)

    {
        std::cerr << "error receiving message from client\n";
        close(client_fd);
        close(server_fd);
        return "";
    }
    return client_message;
}

void ConnectionHandler::handleClient() const {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    if (client_fd < 0) {
        std::cerr << "Failed to accept client connection" << std::endl;
        return;
    } else{
        std::cout << "Client connected\n";
    }
    std::string message = getMessageFromSocket(client_fd);
    std::cout << "Message " << message << std::endl;
    if (message.starts_with("GET / HTTP/1.1\r\n")){
        send(client_fd,"HTTP/1.1 200 OK\r\n\r\n",sizeof("HTTP/1.1 200 OK\r\n\r\n"),0);
    } else{
        send(client_fd,"HTTP/1.1 404 Not Found\r\n\r\n",sizeof("HTTP/1.1 404 NOT FOUND\r\n\r\n"),0);
    }
    close(client_fd);
}