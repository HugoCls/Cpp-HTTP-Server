#pragma once
#include <functional>
#include "request.h"
#include "response.h"

class Route {
public:
    Route(const std::string& url, std::function<Response(const Request&)> handler);
    bool matches(const Request& request) const;
    Response operator()(const Request& request) const;

private:
    std::string url;
    std::function<Response(const Request&)> handler;
};
