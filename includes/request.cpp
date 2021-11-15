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

std::vector<std::string> const &Request::getValue(const std::string &key) {
    return _requestMap[key];
}

void Request::parseRequest(std::stringstream &requestStream) {
    std::string line;
    bool is_body(false);
    while (std::getline(requestStream, line)) {

        if (line.empty()) {
            is_body = true;
            continue;
        }
        if (_isBodyEnd(line)) {
            is_body = false;
            continue;
        };
        if (_isBodyStart(line, is_body)) {
            is_body = true;
            continue;
        }
        if (!is_body) _getHeader(line);
        else _getBody(line);
    }
}

bool Request::_isBodyStart(const std::string &line, bool is_body) const { return is_body && line == "{"; }

bool Request::_isBodyEnd(const std::string &line) const { return line == "}"; }

void Request::_getBody(std::string &line) {
    line.push_back('\n');
    if (_requestMap.count("Body:"))
        _requestMap["Body:"][0] += line;
    else
        _requestMap["Body:"].push_back(line);
}

bool Request::_isBody(std::string const &line, bool const &is_body) const { return line == "{" && is_body; }

void Request::_getHeader(const std::string &line) {
    std::__1::vector<std::string> tokens = Utility::split(line, ' ');
    if (tokens[0] == "GET" || tokens[0] == "POST" || tokens[0] == "DELETE")
        _requestMap["SL"] = std::__1::vector<std::string>(tokens.begin(), tokens.end());
    else
        _requestMap[tokens[0]] = std::__1::vector<std::string>(tokens.begin() + 1, tokens.end());
}

std::map<std::string, std::vector<std::string> > const &Request::getMap() const {
    return _requestMap;
}
