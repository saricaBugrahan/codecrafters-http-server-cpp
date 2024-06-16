//
// Created by bugra on 16.06.2024.
//

#include "../include/HTTPMessageHandler.h"
#include "../include/GzipUtils.h"
const std::string HTTPMessageHandler::HTTP_NOT_FOUND = "HTTP/1.1 404 Not Found\r\n\r\n";
const std::string HTTPMessageHandler::HTTP_SUCCESS = "HTTP/1.1 200 OK\r\n\r\n";
const std::string HTTPMessageHandler::CREATED = "HTTP/1.1 201 Created\r\n\r\n";
const std::string HTTPMessageHandler::APP_CONTENT = "application/octet-stream";
const std::string HTTPMessageHandler::TEXT_CONTENT = "text/plain";
std::string HTTPMessageHandler::directory;
string_vector HTTPMessageHandler::validEncodings = {"gzip", "deflate", "br"};


//TODO: Rearrange the all structure of the HTTPMessageHandler so that it class will be smooth.
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
    if (splitPath[1].empty()){
        handleSuccesCommand(socket_fd);
    }
    else if(splitPath[1] == "echo" && tokens[2].starts_with("Accept-Encoding:")){
        string_vector encoding = splitMessageIntoTokens(tokens[2],"Accept-Encoding: ");
        string_vector encodings = splitMessageIntoTokens(encoding[1],", ");
        std::string validEncoding = getValidEncoding(encodings);
        if (!validEncoding.empty()){

            std::string msg = (gzip_compress(splitPath[2]));
            std::cout << "Mesaj: "<<splitPath[2] << std::endl;
            std::cout << "Gzip Mesaj: " << msg << std::endl;
            sendMessageRespondToSocket(socket_fd, convertStringIntoResponse(
                    msg, "text/plain", HTTP_SUCCESS, true, validEncoding
            ));
        }
        else{
            sendMessageRespondToSocket(socket_fd, convertStringIntoResponse(
                    encoding[1],"text/plain",HTTP_SUCCESS, false,encoding[1]
            ));
        }
    }
    else if (splitPath[1] == "echo"){
        handleEchoCommand(socket_fd,splitPath);
    }

    else if (splitPath[1] == "user-agent"){
        string_vector splitUserAgent = splitMessageIntoTokens(tokens[2],"User-Agent: ");
        handleUserAgentCommand(socket_fd,splitUserAgent);
    }
    else if(splitPath[1] == "files" && splitRequestLine[0] == "GET"){
        handleFileCommand(socket_fd,splitPath);
    }
    else if(splitPath[1] == "files" && splitRequestLine[0] == "POST"){
        handleFileWriteCommand(socket_fd, splitPath[2],const_cast<std::string &>(tokens[4]));
    }

    else{
        handleErrorCommand(socket_fd);
    }
}

void HTTPMessageHandler::handleErrorCommand(int socket_fd) {
    sendMessageRespondToSocket(socket_fd, HTTP_NOT_FOUND);
}

void HTTPMessageHandler::handleEchoCommand(int socket_fd, string_vector &tokens) {
    sendMessageRespondToSocket(socket_fd, convertStringIntoResponse(tokens[2],TEXT_CONTENT,HTTP_SUCCESS,false,""));
}

//TODO: fix typo
void HTTPMessageHandler::handleSuccesCommand(int socket_fd) {
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
std::string HTTPMessageHandler::convertStringIntoResponse(std::string &msg,std::string contentType,std::string httpStatus,bool acceptEncoding,std::string encoding) {
    char buffer[500];
    if (acceptEncoding){
        std::ostringstream oss;
        oss << "HTTP/1.1 200 OK\r\n";
        oss << "Content-Encoding: " << encoding << "\r\n";
        oss << "Content-Type: " << contentType << "\r\n";
        oss << "Content-Length: " << msg.size() << "\r\n";
        oss << "\r\n";
        oss << msg;
        return oss.str();
    }
    else{
        if (contentType == APP_CONTENT){
            sprintf(buffer,"HTTP/1.1 200 OK\r\n"
                           "Content-Type: %s\r\nContent-Length: %zu\r\n\r\n%s\r\n",contentType.c_str(),msg.size(),msg.c_str());
        } else{
            sprintf(buffer,"HTTP/1.1 200 OK\r\n"
                           "Content-Type: %s\r\nContent-Length: %zu\r\n\r\n%s",contentType.c_str(),msg.size(),msg.c_str());
        }
    }
    return buffer;
}

std::string HTTPMessageHandler::getValidEncoding(const string_vector &encodings) {
    for (const std::string &encoding : encodings) {
        if (encoding == "gzip") {
            return encoding;
        }
    }
    return "";
}






