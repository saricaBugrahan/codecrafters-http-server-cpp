//
// Created by bugra on 16.06.2024.
//

#include "../include/HTTPMessageHandler.h"


string_vector HTTPMessageHandler::splitMessageIntoTokens(const std::string &message,const std::string delim) {
    string_vector tokens;
    std::string token;
    size_t start = 0,end;
    while ((end = message.find(delim)) != std::string::npos){
        tokens.push_back(message.substr(start,end-start));
        start = end + delim.length();
    }
    tokens.push_back(message.substr(start));
    return tokens;
}

int HTTPMessageHandler::sendMessageToSocket(int socket,std::string &buffer) {
    ssize_t result = send(socket,buffer.c_str(),buffer.size(),0);
    if (result < 0){
        std::cerr << "Cannot send the message to the socket" << std::endl;
        return 1;
    }
    return 0;
}
