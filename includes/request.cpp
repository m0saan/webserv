//
// Created by Mohammad  Boustta on 11/12/21.
//

#include "request.hpp"
#include "utility.hpp"
#include <iostream>

// Request::Request() {}

Request::~Request() {}

Request::Request(const Request &x) {
    *this = x;
}

Request &Request::operator=(const Request &x) {
//    this->_RequestMap = x._RequestMap;
    return *this;
}

std::vector<std::string> const &Request::getValue(const std::string &key) {
    return _RequestMap[key];
}

void Request::parseRequest(std::stringstream &RequestStream) {
    std::string line;
    bool is_body(false);
    while (std::getline(RequestStream, line)) {

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
    if (_RequestMap.count("Body:"))
        _RequestMap["Body:"][0] += line;
    else
        _RequestMap["Body:"].push_back(line);
}

bool Request::_isBody(std::string const &line, bool const &is_body) const { return line == "{" && is_body; }

void Request::_getHeader(const std::string &line) {
    std::__1::vector<std::string> tokens = Utility::split(line, ' ');
    if (tokens[0] == "GET" || tokens[0] == "POST" || tokens[0] == "DELETE")
        _RequestMap["SL"] = std::__1::vector<std::string>(tokens.begin(), tokens.end());
    else
        _RequestMap[tokens[0]] = std::__1::vector<std::string>(tokens.begin() + 1, tokens.end());
}

std::map<std::string, std::vector<std::string> > const &Request::getMap() const {
    return _RequestMap;
}


Request::Request(long long max_size ):_size(-1), _content_length(-1), _header_length(-1), _max_body_size(max_size)
{
}

std::string Request::get_req()const
{
	return this->_req;
}

bool	Request::is_completed() const
{
	return (_size == _content_length + _header_length);
}

void 	Request::append(char *content, long long size)
{
	std::string tmp(content, size);
	if (_content_length == -1) // 1st time reading req
		getReqInfo(std::string(content, size));
	_req += tmp;
	_size = _req.length();
}

void Request::getReqInfo(std::string str)
{
	_content_length = getContentLength(str);
	_header_length = getHeaderLength(str);
}

size_t Request::getHeaderLength(std::string str)
{
	size_t pos = 0;

	if ((pos = str.find("\r\n\r\n")) != std::string::npos)
		_header_length = (pos + 5);
	else
		throw std::exception();
	return pos;
}

long long Request::getContentLength(std::string str)
{
	size_t pos;
	long long length = -1;
	char *ptr_end;

	if ((pos = str.find("Content-Length: ")) != std::string::npos)
	{
		length = std::strtoll(&str[pos + 16], &ptr_end, 10) + 4;
		if (ptr_end == &str[pos + 16])
			throw std::exception();
		std::cout << "Content-Length : " << length << std::endl;
		this->transfer_encoding = COMPLETED;
		if (length > _max_body_size) // check if lenght is greater than max body size
			throw std::exception();
		this->transfer_encoding = COMPLETED;
	}
	/*

			else maybe it's a chunked req or bad Request

	*/
	
	return length;
}