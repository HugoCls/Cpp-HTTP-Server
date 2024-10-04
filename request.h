#pragma once
#include <iostream>
#include <map>
#include <string>

class Request
{
public:
    enum verb {
		GET,
        POST,
        PUT,
        DELETE,
        PATCH,
        OPTIONS,
        HEAD
	};

private:
	verb httpverb;
	std::string url;
	std::string body;
	std::map<std::string, std::string> headers;  // Dictionnaire pour stocker les headers

public:
    Request();
	const verb& getverb() const;
	const std::string& geturl() const;
	const std::string& getbody() const;
	const std::map<std::string, std::string>& getheaders() const;
	void setverb(verb v);
	void seturl(const std::string& url);
	void setbody(const std::string& body);
	
    std::map<std::string, std::string>::iterator begin();
    std::map<std::string, std::string>::iterator end();
    
	friend std::ostream& operator<<(std::ostream& stream, const Request& req);
	friend std::istream& operator>>(std::istream& stream, Request& req);
	friend std::string verbToString(Request::verb v);
	friend Request::verb stringToVerb(const std::string& method);
};