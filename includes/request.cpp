//
// Created by Mohammad  Boustta on 11/12/21.
//

#include "request.hpp"
#include "utility.hpp"
#include <iostream>

Request::~Request() {}

Request::Request(const Request &x)
{
	_size = x._size;
	_req.clear();
	_content_length = x._content_length;
	_header_length = x._header_length;
	_max_body_size = x._max_body_size;
	_transfer_encoding = x._transfer_encoding;
}

// Request &Request::operator=(const Request &x) {
// //    this->_RequestMap = x._RequestMap;
//     return *this;
// }

void Request::resetRequest()
{
	_req.clear();
	_req.str("");
	_req.clear();
	_size = -1;
	_content_length = -1;
	_header_length = -1;
}

std::vector<std::string> const &Request::getValue(const std::string &key)
{
	return _RequestMap[key];
}

void Request::parseRequest()
{
	std::string line;
	bool is_body(false);
	bool is_chunked(false);
	while (std::getline(_req, line))
	{
		line.pop_back();
		if (line.empty() || _isBodyStart(line, is_body))
		{
			is_body = true;
			continue;
		}
		if (_isBodyEnd(line))
		{
			is_body = false;
			continue;
		};

		if (_isChunckStart(line))
		{
			is_chunked = true;
			continue;
		}

		if (!is_body)
			_getHeader(line);
		else
			_getBody(line, is_chunked);
	}
	if (_RequestMap.count("Connection"))
		_is_alive_connection = _RequestMap["Connection"][0] == "keep-alive";
}

bool Request::_isChunckStart(std::string const &line) const
{
	return (line.length() > 0 && isnumber(line[0]));
}

bool Request::_isBodyStart(const std::string &line, bool is_body) const { return is_body && line == "{"; }

bool Request::_isBodyEnd(const std::string &line) const { return line == "}"; }

void Request::_getBody(std::string &line, bool is_chunked)
{
	line.push_back('\n');
	static int i = 0;
	if (is_chunked)
		if (i % 2 != 0)
			return;
	if (_RequestMap.count("body"))
		_RequestMap["body"][0] += line;
	else
		_RequestMap["body"].push_back(line);
}

bool Request::_isBody(std::string const &line, bool const &is_body) const { return line == "{" && is_body; }

std::map<std::string, std::vector<std::string> > const &Request::getMap() const
{
	return _RequestMap;
}

Request::Request(long long max_size) : _req(), _size(-1), _content_length(-1), _header_length(-1), _max_body_size(max_size)
{
}

// Request::request(char *content, long long lenght, long long content_length):_req(std::string(content, lenght)), _size(lenght), _content_length(content_length)
// {
// }

std::stringstream const &Request::get_req()
{
	return this->_req;
}

bool	Request::is_completed() const
{
	if (_transfer_encoding == COMPLETED)
	{
		if ((_content_length == -1 || _content_length == 0))
			return _size == _header_length + 4;
		return (_size == _content_length + _header_length);
	}
	else if (_transfer_encoding == CHUNKED)
	{
		if(_req.str().find("0\r\n\r\n") != std::string::npos)
			return true;
		else if (_req.str().length() == _req.str().find("\r\n\r\n") + 4) // chunked request without body
			return true;
		return false;
	}
	return true;
}

void Request::append(char *content, long long size)
{
	std::string tmp(content, size);
	if (_content_length == -1) // 1st time reading req
		try
		{
			getReqInfo(tmp);
		}
		catch (std::exception &e)
		{
			std::cout << "Bad Request !" << std::endl;
			return;
		}
	_req << content;
	_size = _req.str().length();
}

void Request::getReqInfo(const std::string &str)
{
	_content_length = getContentLength(str);
	_header_length = getHeaderLength(str);
}

std::pair<std::string, std::string> _parseStartLine(std::string& url) {
	std::string queries;
    std::pair<bool, int> has_queries_result;
    has_queries_result = Utility::hasQueries(url);
    if (has_queries_result.first)
        queries =  Utility::getQueries(url, has_queries_result.second);
	std::string script_name = Utility::getScriptName(url);
	return std::make_pair(queries, script_name);
}

void Request::_getHeader(const std::string &line)
{
	std::vector<std::string> tokens = Utility::split(line, ' ');
	if (tokens[0] == "GET" || tokens[0] == "POST" || tokens[0] == "DELETE"){
		_RequestMap["SL"] = std::vector<std::string>(tokens.begin(), tokens.end());
		_url_queries_scriptName = _parseStartLine(tokens[1]);
	}
	else
	{
		tokens[0].pop_back();
		_RequestMap[tokens[0]] = std::vector<std::string>(tokens.begin()+1, tokens.end());
	}
}

size_t Request::getHeaderLength(const std::string &str)
{
	size_t pos = 0;

	if ((pos = str.find("\r\n\r\n")) != std::string::npos)
		_header_length = (pos + 5);
	else
	{
		throw std::exception();
	}
	return pos;
}

long long Request::getContentLength(const std::string& str)
{
	size_t pos;
	long long length = 0;
	char *ptr_end;

	if (_content_length == -1 && (pos = str.find("Content-Length: ")) != std::string::npos) // if content-Length is found
	{
		length = std::strtoll(&str[pos + 16], &ptr_end, 10) + 4;
		if (ptr_end == &str[pos + 16])
			throw std::exception();
		// if (length > _max_body_size) // check if lenght is greater than max body size
		// 	throw std::exception();
		this->_transfer_encoding = COMPLETED;
	}
	else if (_content_length == -1 && (str.find("Transfer-Encoding: chunked")) !=  std::string::npos) // !content-Lenght && transfer-Encoding = chunked
		this->_transfer_encoding = CHUNKED;
	// else // Content-Length not found && !chunked  && should be POST or DELETE
	// 	throw std::exception();
	return length;
}

std::pair<std::string, std::string> &Request::getQueriesScriptName() {
    return _url_queries_scriptName;
}
