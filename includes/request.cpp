//
// Created by Mohammad  Boustta on 11/12/21.
//

#include "request.hpp"
#include "utility.hpp"
#include <iostream>

Request::Request() {}

Request::~Request() {}

Request::Request(const Request &x) {
    *this = x;
}

Request &Request::operator=(const Request &x) {
//    this->_requestMap = x._requestMap;
    return *this;
}

std::vector<std::string> const& Request::getValue(const std::string &key) {
    return _requestMap[key];
}

void Request::parseRequest(std::stringstream &requestStream) {
    std::string line;
    while (std::getline(requestStream, line)) {
        std::vector<std::string> tokens = Utility::split(line, ' ');
        if (tokens[0] == "GET" || tokens[0] == "POST" || tokens[0] == "DELETE")
            _requestMap["SL"] = std::vector<std::string>(tokens.begin(), tokens.end());
        else
            _requestMap[ tokens[0] ] = std::vector<std::string>(tokens.begin() + 1, tokens.end());
    }
}

std::map<const std::string, std::vector<std::string> > const &Request::getMap() const {
    return _requestMap;
}
