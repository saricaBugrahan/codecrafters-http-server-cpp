//
// Created by bugra on 17.06.2024.
//

#ifndef HTTP_SERVER_STARTER_CPP_HTTPMESSAGE_H
#define HTTP_SERVER_STARTER_CPP_HTTPMESSAGE_H


#include <string>

class HTTPMessage {
public:
    HTTPMessage(const std::string &requestMethod, const std::string &command);
    std::string getCommand();
    std::string getEncodingType();
    std::string getRequestMethod();
    [[nodiscard]] bool getHasEncoding() const;

    void setEncodingType(std::string encoding);
    void setRequestMethod(std::string request);
    void setHasEncoding(bool hasEncode);

private:
    std::string requestMethod;
    std::string command;
    std::string encodingType;
    bool hasEncoding = false;

};


#endif //HTTP_SERVER_STARTER_CPP_HTTPMESSAGE_H
