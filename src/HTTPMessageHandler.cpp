//
// Created by bugra on 16.06.2024.
//

#include "../include/HTTPMessageHandler.h"
#include "../include/GzipUtils.h"
#include "../include/HTTPMessage.h"


const std::string HTTPMessageHandler::HTTP_NOT_FOUND = "HTTP/1.1 404 Not Found\r\n\r\n";
const std::string HTTPMessageHandler::HTTP_SUCCESS = "HTTP/1.1 200 OK\r\n\r\n";
const std::string HTTPMessageHandler::CREATED = "HTTP/1.1 201 Created\r\n\r\n";
const std::string HTTPMessageHandler::APP_CONTENT = "application/octet-stream";
const std::string HTTPMessageHandler::TEXT_CONTENT = "text/plain";
std::string HTTPMessageHandler::directory;


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
//TODO: rearrange the structure in a struct so that it will not be needed to the indexing.
void HTTPMessageHandler::handleResponseType(int socket_fd,const string_vector& tokens) {
    string_vector splitRequestLine = splitMessageIntoTokens(tokens[0], " ");
    string_vector splitPath = splitMessageIntoTokens(splitRequestLine[1],"/");

    HTTPMessage httpMessage(splitRequestLine[0],splitPath[1]);

    if (httpMessage.getCommand().empty()){
        handleSuccessCommand(socket_fd);
        return;
    }

    if (tokens[2].starts_with("Accept-Encoding")){
        httpMessage.setHasEncoding(true);
    }

    if (httpMessage.getCommand() == "echo"){
        if (!httpMessage.getHasEncoding()){
            handleEchoCommand(socket_fd,splitPath);
            return;
        }
        string_vector encoding = splitMessageIntoTokens(tokens[2],"Accept-Encoding: ");
        string_vector encodings = splitMessageIntoTokens(encoding[1],", ");
        std::string validEncoding = getValidEncoding(encodings);
        if (!validEncoding.empty()){
            sendMessageRespondToSocket(socket_fd, convertStringIntoResponse(
                    gzip_compress(splitPath[2]), "text/plain", HTTP_SUCCESS, true, validEncoding
            ));
        }
        else{
            sendMessageRespondToSocket(socket_fd, convertStringIntoResponse(
                    encoding[1],"text/plain",HTTP_SUCCESS, false,encoding[1]
            ));
        }
        return;
    }


    if (httpMessage.getCommand() == "user-agent"){
        string_vector splitUserAgent = splitMessageIntoTokens(tokens[2],"User-Agent: ");
        handleUserAgentCommand(socket_fd,splitUserAgent);
        return;
    }

    if (httpMessage.getCommand() == "files"){
        if (httpMessage.getRequestMethod() == "GET"){
            handleFileCommand(socket_fd,splitPath);
        } else{
            handleFileWriteCommand(socket_fd, splitPath[2],const_cast<std::string &>(tokens[4]));
        }
        return;
    }


    handleErrorCommand(socket_fd);

}

void HTTPMessageHandler::handleErrorCommand(int socket_fd) {
    sendMessageRespondToSocket(socket_fd, HTTP_NOT_FOUND);
}

void HTTPMessageHandler::handleEchoCommand(int socket_fd, string_vector &tokens) {
    sendMessageRespondToSocket(socket_fd, convertStringIntoResponse(tokens[2],TEXT_CONTENT,HTTP_SUCCESS,false,""));
}

void HTTPMessageHandler::handleSuccessCommand(int socket_fd) {
    sendMessageRespondToSocket(socket_fd,HTTP_SUCCESS);
}
void HTTPMessageHandler::handleUserAgentCommand(int socket_fd, string_vector &tokens) {
    sendMessageRespondToSocket(socket_fd, convertStringIntoResponse(tokens[1],TEXT_CONTENT,HTTP_SUCCESS, false,""));
}

void HTTPMessageHandler::handleCreatedCommand(int socket_fd) {
    sendMessageRespondToSocket(socket_fd,CREATED);
}


void HTTPMessageHandler::handleFileCommand(int socket_fd, string_vector &tokens) {
    std::ifstream ifs(directory+tokens[2]);
    if (!ifs.is_open()) {
        handleErrorCommand(socket_fd);
        return;
    }
    std::string content;
    std::string line;
    while (std::getline(ifs, line)) {
        content += line + '\n';
    }
    if (!content.empty() && content.back() == '\n') {
        content.pop_back();
    }
    sendMessageRespondToSocket(socket_fd, convertStringIntoResponse(content,APP_CONTENT,HTTP_SUCCESS, false,""));

}

//TODO: read according to the data length not by the '\0'
void HTTPMessageHandler::handleFileWriteCommand(int socket_fd, std::string &filename,std::string &msg) {
    std::ofstream ofs(directory+filename);
    for(const char &x : msg){
        if (x == 0){
            break;
        }
        ofs << x;
    }
    ofs.close();
    handleCreatedCommand(socket_fd);
}


//TODO: fix into one structure so that it will be created with parameters
std::string HTTPMessageHandler::convertStringIntoResponse(std::basic_string<char> msg, std::string contentType, std::string httpStatus, bool acceptEncoding, std::string encoding) {
    std::ostringstream oss;
    if (acceptEncoding){
        oss << "HTTP/1.1 200 OK\r\n";
        oss << "Content-Encoding: " << encoding << "\r\n";
        oss << "Content-Type: " << contentType << "\r\n";
        oss << "Content-Length: " << msg.size() << "\r\n";
        oss << "\r\n";
        oss << msg;
    }
    else{
        oss << "HTTP/1.1 200 OK\r\n";
        oss << "Content-Type: " << contentType << "\r\n";
        oss << "Content-Length: " << msg.size() << "\r\n";
        oss << "\r\n";
        oss << msg;
    }
    return oss.str();;
}

std::string HTTPMessageHandler::getValidEncoding(const string_vector &encodings) {
    for (const std::string &encoding : encodings) {
        if (encoding == "gzip") {
            return encoding;
        }
    }
    return "";
}






