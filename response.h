#pragma once
#include <string>
#include <map>
#include <ostream>

class Response {
public:
    void setStatus(int statusCode);
    void setStatusText(const std::string& statusText);
    void setBody(const std::string& body);
    void addHeader(const std::string& key, const std::string& value);
    std::string toString() const;

    friend std::ostream& operator<<(std::ostream& stream, const Response& res);

private:
    int status = 200;
    std::string statusText = "OK";
    std::map<std::string, std::string> headers;
    std::string body;
};
