#include "router.h"

void Router::addRoute(Route* route) {
    routes.push_back(route);
}

Response Router::serveRequest(const Request& request) const {
    for (const auto& route : routes) {
        if (route->matches(request)) {
            return (*route)(request);
        }
    }
    Response res;
    res.setStatus(404);
    res.setStatusText("Not Found");
    return res;
}
