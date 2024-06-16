#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "ConnectionHandler.h"
int main(int argc, char **argv) {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  ConnectionHandler connectionHandler;

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

   struct sockaddr_in client_addr;
   int client_addr_len = sizeof(client_addr);

   std::cout << "Waiting for a client to connect...\n";

   int client = accept(connectionHandler.server_fd, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len);
   std::string msg = "HTTP/1.1 200 OK\r\n\r\n";
   send(client,msg.c_str(),msg.size(),0);
  std::cout << "Client connected\n";

   close(connectionHandler.server_fd);

  return 0;
}
