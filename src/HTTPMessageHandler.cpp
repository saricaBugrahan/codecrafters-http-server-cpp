//
// Created by bugra on 16.06.2024.
//

#include "../include/HTTPMessageHandler.h"

const std::string HTTPMessageHandler::HTTP_NOT_FOUND = "HTTP/1.1 404 Not Found\r\n\r\n";
const std::string HTTPMessageHandler::HTTP_SUCCESS = "HTTP/1.1 200 OK\r\n\r\n";

string_vector HTTPMessageHandler::splitMessageIntoTokens(const std::string &message,const std::string& delim) {
    string_vector tokens;
    std::string token;
    size_t start = 0,end;
    while ((end = message.find(delim,start)) != std::string::npos){
        tokens.push_back(message.substr(start,end-start));
        start = end + delim.length();
    }
    tokens.push_back(message.substr(start));
    return tokens;
}

int HTTPMessageHandler::sendMessageRespondToSocket(int socket, std::string buffer) {
    ssize_t result = send(socket,buffer.c_str(),buffer.size(),0);
    if (result < 0){
        std::cerr << "Cannot send the message to the socket" << std::endl;
        return 1;
    }
    return 0;
}

void HTTPMessageHandler::handleResponseType(int socket_fd,const string_vector& tokens) {
    string_vector splitRequestLine = splitMessageIntoTokens(tokens[0], " ");
    string_vector splitPath = splitMessageIntoTokens(splitRequestLine[1],"/");
    if (splitPath[1].empty()){
        handleSuccesCommand(socket_fd);
    }
    else if(splitPath[1] == "echo"){
        handleEchoCommand(socket_fd,splitPath);
    }
    else if (splitPath[1] == "user-agent"){
        string_vector splitUserAgent = splitMessageIntoTokens(tokens[2],"User-Agent: ");
        handleUserAgentCommand(socket_fd,splitUserAgent);

    }
    else{
        handleErrorCommand(socket_fd);
    }
}

void HTTPMessageHandler::handleErrorCommand(int socket_fd) {
    sendMessageRespondToSocket(socket_fd, HTTP_NOT_FOUND);
}

void HTTPMessageHandler::handleEchoCommand(int socket_fd, string_vector &tokens) {
    sendMessageRespondToSocket(socket_fd, convertStringIntoResponse(tokens[2]));
}

void HTTPMessageHandler::handleSuccesCommand(int socket_fd) {
    sendMessageRespondToSocket(socket_fd,HTTP_SUCCESS);
}
void HTTPMessageHandler::handleUserAgentCommand(int socket_fd, string_vector &tokens) {
    sendMessageRespondToSocket(socket_fd, convertStringIntoResponse(tokens[1]));
}

std::string HTTPMessageHandler::convertStringIntoResponse(std::string &msg) {
    char buffer[100];
    sprintf(buffer,"HTTP/1.1 200 OK\r\n"
                   "Content-Type: text/plain\r\nContent-Length: %zu\r\n\r\n%s",msg.size(),msg.c_str());
    return buffer;
}





