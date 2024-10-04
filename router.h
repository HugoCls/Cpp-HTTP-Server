#pragma once
#include <vector>
#include "request.h"
#include "response.h"
#include "route.h"

class Router {
public:
    void addRoute(Route* route);
    Response serveRequest(const Request& request) const;

private:
    std::vector<Route*> routes;
};
