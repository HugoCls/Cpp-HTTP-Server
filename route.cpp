#include "route.h"

Route::Route(const std::string& url, std::function<Response(const Request&)> handler)
    : url(url), handler(handler) {}

bool Route::matches(const Request& request) const {
    return request.geturl() == url;
}

Response Route::operator()(const Request& request) const {
    return handler(request);
}
