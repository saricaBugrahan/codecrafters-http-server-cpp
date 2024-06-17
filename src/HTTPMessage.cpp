//
// Created by bugra on 17.06.2024.
//

#include <utility>

#include "../include/HTTPMessage.h"

HTTPMessage::HTTPMessage(const std::string &requestMethod, const std::string &command) {
    this->requestMethod = requestMethod;
    this->command = command;
}

std::string HTTPMessage::getCommand() {
    return this->command;
}

std::string HTTPMessage::getEncodingType() {
    return this->encodingType;
}

std::string HTTPMessage::getRequestMethod() {
    return this->requestMethod;
}
bool HTTPMessage::getHasEncoding() const {
    return this->hasEncoding;
}

void HTTPMessage::setEncodingType(std::string encoding) {
    this->encodingType = std::move(encoding);
}

void HTTPMessage::setRequestMethod(std::string request) {
    this->requestMethod = std::move(request);
}

void HTTPMessage::setHasEncoding(bool hasEncode) {
    this->hasEncoding = hasEncode;
}






