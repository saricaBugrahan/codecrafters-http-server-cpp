#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../include/ConnectionHandler.h"
int main(int argc, char **argv) {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;
    ConnectionHandler connectionHandler{};
    if (argc == 3 && std::string(argv[1]) == "--directory"){
       HTTPMessageHandler::directory = std::string(argv[2]);
    }

    if (connectionHandler.createServerSocket() == ERROR){
        return 1;
    }
    if (connectionHandler.setServerSocketForReuse() == ERROR){
        return 1;
    }

    if (connectionHandler.bindServerSocketToPort(4221) == ERROR){
        return 1;
    }
    if (connectionHandler.startListeningThePort() == ERROR){
        return 1;
    }


    std::cout << "Waiting for a client to connect...\n";
    while (true){
        connectionHandler.handleClient();
    }
    close(connectionHandler.server_fd);

    return 0;
}
