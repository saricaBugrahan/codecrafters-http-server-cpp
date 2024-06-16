//
// Created by bugra on 16.06.2024.
//

#ifndef HTTP_SERVER_STARTER_CPP_CONNECTIONHANDLER_H
#define HTTP_SERVER_STARTER_CPP_CONNECTIONHANDLER_H


enum MSG{OK=0,ERROR=1};

class ConnectionHandler {

public:
    int server_fd;
    ConnectionHandler() = default;
    int createServerSocket();
    int setServerSocketForReuse() const;
    int bindServerSocketToPort(int port) const;
    int startListeningThePort();
};


#endif //HTTP_SERVER_STARTER_CPP_CONNECTIONHANDLER_H
