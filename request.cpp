#include <iostream>                  // Pour std::cout, std::istream, std::ostream
#include <map>                       // Pour std::map
#include <string>                    // Pour std::string
#include <boost/algorithm/string.hpp> // Pour les fonctions boost::trim et boost::to_upper
#include <boost/asio.hpp>            // Pour boost::asio si utilisé ailleurs
#include <boost/beast/http.hpp>      // Pour les types HTTP, boost::beast::http
#include <boost/exception/all.hpp>   // Pour les exceptions Boost, si elles sont utilisées
#include "request.h"

const Request::verb& Request::getverb() const {
    return httpverb;
}

const std::string& Request::geturl() const {
    return url;
}

const std::string& Request::getbody() const {
    return body;
}

const std::map<std::string, std::string>& Request::getheaders() const {
    return headers;
}

void Request::setverb(verb v) {
    httpverb = v;
}

void Request::seturl(const std::string& new_url) {
    url = new_url;
}
void Request::setbody(const std::string& new_body) {
    body = new_body;
}

std::map<std::string, std::string>::iterator Request::begin() {
    return headers.begin();
}

std::map<std::string, std::string>::iterator Request::end() {
    return headers.begin();
}

Request::Request() : httpverb(Request::GET), url{""}, body{""}, headers{} {}

Request::verb stringToVerb(const std::string& method) {
    if (method == "GET") return Request::GET;
    else if (method == "POST") return Request::POST;
    else if (method == "PUT") return Request::PUT;
    else if (method == "DELETE") return Request::DELETE;
    else if (method == "PATCH") return Request::PATCH;
    else if (method == "OPTIONS") return Request::OPTIONS;
    else if (method == "HEAD") return Request::HEAD;
    else return Request::GET;
}

std::string verbToString(Request::verb v) {
    switch (v) {
        case Request::GET: return "GET";
        case Request::POST: return "POST";
        case Request::PUT: return "PUT";
        case Request::DELETE: return "DELETE";
        case Request::PATCH: return "PATCH";
        case Request::OPTIONS: return "OPTIONS";
        case Request::HEAD: return "HEAD";
        default: return "UNKNOWN";
    }
}


std::ostream& operator<<(std::ostream& stream, const Request& req) {
    
    stream << (verbToString(req.httpverb))
       << " " << req.geturl() << "\n";

    for (const auto& header : req.getheaders()) {
        stream << header.first << ": " << header.second << "\n";
    }

    stream  << "\n" << req.getbody() << "\n";

    return stream;
}


std::istream& operator>>(std::istream& is, Request& req) {
std::string line;
enum Step {
    INIT = 0,
    HEADERS = 1,
    BODY = 2,
    DONE = 3,
};

int step = 0;
while (step < Step::DONE) {
    if (!std::getline(is, line, '\n')) throw std::runtime_error("Bad Request 1");
    boost::trim(line);
    
    if (line.size() == 0) {
    if (step == Step::HEADERS) {
        switch (req.httpverb) { // Dans le cas de requêtes non POST/PUT, on passe directement à DONE (pas de lecture du BODY)
        case Request::verb::POST:
        case Request::verb::PUT:
            step +=1;
            break;
        default:
            step = DONE;
        }
    } else {
        step += 1;
    }
    }

    switch (step) {
    case INIT: {
        int delimiter = line.find(' ');
        std::string verb = line.substr(0, delimiter);
        line = line.substr(delimiter + 1); 
        boost::to_upper(verb);

        // VERB
        if (verb == "GET") req.httpverb = Request::verb::GET;
        else if (verb == "POST") req.httpverb = Request::verb::POST;
        else if (verb == "PUT") req.httpverb = Request::verb::PUT;
        else if (verb == "DELETE") req.httpverb = Request::verb::DELETE;
        else if (verb == "HEAD") req.httpverb = Request::verb::HEAD;
        else if (verb == "OPTION") req.httpverb = Request::verb::OPTIONS;
        else throw std::runtime_error("Bad Request 2");

        // ROUTE
        delimiter = line.find(' ');
        req.url = line.substr(0, delimiter); 
        boost::trim(req.url);

        step += 1;
        break;
    }
        
    case HEADERS: {
        int delimiter = line.find(':');
        std::string header = line.substr(0, delimiter);
        std::string value = line.substr(delimiter + 2);
        boost::trim(header); boost::to_lower(header);
        boost::trim(value); boost::to_lower(value);
        if (header.size() > 0) req.headers.insert(std::pair{header, value});
        break;
    }

    case BODY: {
        req.body += std::move(line);
        break;
    }
    
    case DONE:
    default:
        return is;
    }
}

return is;
}
