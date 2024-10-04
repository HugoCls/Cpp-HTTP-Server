#include "response.h"

void Response::setStatus(int statusCode) {
    status = statusCode;
}

void Response::setStatusText(const std::string& statusText) {
    this->statusText = statusText;
}

void Response::setBody(const std::string& body) {
    this->body = body;
    addHeader("Content-Length", std::to_string(body.size()));
}

void Response::addHeader(const std::string& key, const std::string& value) {
    headers[key] = value;
}

std::string Response::toString() const {
    std::string res = "HTTP/1.1 " + std::to_string(status) + " " + statusText + "\r\n";
    for (const auto& header : headers) {
        res += header.first + ": " + header.second + "\r\n";
    }
    res += "\r\n" + body;
    return res;
}

std::ostream& operator<<(std::ostream& stream, const Response& res) {
    return stream << res.toString();
}