//
// Created by Mohammad  Boustta on 11/12/21.
//

#include "../includes/request.hpp"
#include "../includes/utility.hpp"
#include <iostream>

Request::Request(long long max_size) : _size(-1), _content_length(-1), _header_length(-1), _content_type(false), _max_body_size(max_size), _is_chunked_completed(false), _req_method(POST), _is_alive_connection(true)
{
	_is_forbiden_method = false;
	_bad_request_found = false;
	_is_chunked_completed = false;
	_allowed_http_methods.push_back("GET");
	_allowed_http_methods.push_back("POST");
	_allowed_http_methods.push_back("DELETE");
}

Request::~Request() {}

Request::Request(const Request &x) : _is_alive_connection(x._is_alive_connection)
{
	_size = x._size;
	_content_length = x._content_length;
	_header_length = x._header_length;
	_max_body_size = x._max_body_size;
	_transfer_encoding = x._transfer_encoding;
	_content_type = x._content_type;
	_is_chunked_completed = x._is_chunked_completed;
	_allowed_http_methods = x._allowed_http_methods;
	_is_forbiden_method = x._is_forbiden_method;
	_url_queries_scriptName = x._url_queries_scriptName;
	_fd = x._fd;
	_bad_request_found = x._bad_request_found;
	_RequestMap = x._RequestMap;
	_req_filename = x._req_filename;
	_req_method = x._req_method;
	_req_header = x._req_header;
}

// Request &Request::operator=(const Request &x) {
// //    this->_RequestMap = x._RequestMap;
//     return *this;
// }

void Request::resetRequest()
{
	_size = -1;
	_content_length = -1;
	_header_length = -1;
	_content_type = false;
	_is_chunked_completed = false;
	_req_method = POST;
	_req_header = "";
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
	bool is_form_data(false);
	std::string http_method;
	std::string boundary;
	int total_read = 0;

	system("rm -f /tmp/body"); // remove the file if it's existe
	_body_stream.open("/tmp/body", std::fstream::in | std::fstream::out | std::fstream::app);
	std::fstream ifs;
	ifs.open(_req_filename, std::ios::in);
	while (std::getline(ifs, line))
	{
		if (_is_forbiden_method)
		{
			ifs.close();
			_body_stream.close();
			system("rm -f /tmp/body");								  // remove the file if it's existe
			system((std::string("rm -rf ") + _req_filename).c_str()); // remove the file if it's existe
			return;
		}

		if (!line.empty() && line.at(line.length() - 1) == '\r')
			line.pop_back();

		if (!is_body && line.empty())
		{
			is_body = true;
			continue;
		}

		if (_isChunckStart(line))
		{
			is_chunked = true;
			continue;
		}

		if (!boundary.empty() && line.find(boundary) != std::string::npos)
		{
			is_form_data = true;
			is_body = false;
			continue;
		}
		if (!is_body)
		{
			std::vector<std::string> res = Utility::split(line);
			std::size_t key_length = res.at(0).length() - 1;
			if ((!res.empty() && std::isupper(res.at(0).at(0)) && res.at(0).at(key_length) == ':') || !_RequestMap.count("SL"))
				_getHeader(line, http_method, boundary);
		}
		else
			_getBody(line, is_chunked, total_read);
	}
	_body_stream.close();

	_checkForBadRequest();

	_fd = !total_read ? -1 : open("/tmp/body", O_RDONLY);

	if (_RequestMap.count("Connection"))
		_is_alive_connection = _RequestMap["Connection"][0] != "close";
	_body_stream.close();
	ifs.close();
	std::remove((_req_filename).c_str());
}

bool Request::_isChunckStart(std::string const &line) const
{
	return (line.length() > 0 && isnumber(line[0]));
}

void Request::_getBody(std::string &line, bool is_chunked, int &total_read)
{
	line.push_back('\n');
	static int i = 0;
	if (is_chunked)
		if (i % 2 != 0)
			return;
	total_read += line.length();
	_body_stream << line;
}

bool Request::_isBody(std::string const &line, bool const &is_body) const { return line == "{" && is_body; }

std::map<std::string, std::vector<std::string> > const &Request::getMap() const
{
	return _RequestMap;
}

bool Request::is_completed() const
{
	if (_req_method == GET)
		return _size >= _header_length + 4;
	if (_transfer_encoding == COMPLETED)
	{
		if (_content_length == -1 || _content_length == 0) // completed request without body
			return _size == _header_length + 4;
		return (_size == _content_length + _header_length);
	}
	else if (_transfer_encoding == CHUNKED)
		return _is_chunked_completed;
	return true;
}

void Request::append(char *content, long long size, int fd)
{
	std::string tmp(content, size);
	if (_req_filename.empty())
		_req_filename = generateFilename(fd);
	_req_file.open(_req_filename, std::fstream::in | std::fstream::out | std::fstream::app);
	_req_header.append(tmp);
	if (_header_length == -1) // 1st time reading req
		try
		{
			getReqInfo();
		}
		catch (std::exception &e)
		{
			std::cout << "Bad Request !" << std::endl;
			_req_file.close();
			return;
		}
	if (_req_file.is_open())
		_req_file << std::string(content, size);
	else
		std::cout << "Cannot open file! " << std::endl;
	_size = _req_file.tellg();
	_req_file.close();
	if ((_transfer_encoding == CHUNKED) &&
		std::string(content).find("0\r\n\r\n") != std::string::npos) // find end message
		this->_is_chunked_completed = true;
}

void Request::getReqInfo()
{
	_req_method = (_req_header.substr(0, _req_header.find(" ")) == "GET") ? GET : POST;
	_content_length = getContentLength();
	_header_length = getHeaderLength();
	setContentType();
}

std::pair<std::string, std::string> _parseStartLine(std::string &url)
{
	std::string queries;
	std::pair<bool, int> has_queries_result;
	has_queries_result = Utility::hasQueries(url);
	if (has_queries_result.first)
		queries = Utility::getQueries(url, has_queries_result.second);
	std::string script_name = Utility::getScriptName(url);
	return std::make_pair(queries, script_name);
}

void Request::_getHeader(const std::string &line, std::string &http_method, std::string &boundary)
{
	std::vector<std::string> tokens = Utility::split(line, ' ');

	if (_RequestMap.count("SL") == 0)
	{
		if (tokens.size() && std::find(_allowed_http_methods.begin(), _allowed_http_methods.end(), tokens[0]) == _allowed_http_methods.end())
		{
			std::cout << tokens[0] << std::endl;
			_is_forbiden_method = true;
			return;
		}
	}

	// Content-Type: multipart/form-data; boundary=--------------------------590098799345060955619546
	if (tokens.size() > 2 && http_method == "POST" && tokens[0] == "Content-Type:" && _RequestMap["SL"][1] == "/upload" && boundary.empty())
	{
		boundary = Utility::split(tokens[2], '=')[1];
		tokens.erase(tokens.begin() + tokens.size() - 1);
	}

	if (tokens[0] == "GET" || tokens[0] == "POST" || tokens[0] == "DELETE")
	{
		_RequestMap["SL"] = std::vector<std::string>(tokens.begin(), tokens.end());
		http_method = tokens[0];
		_url_queries_scriptName = _parseStartLine(tokens[1]);
	}
	else
	{
		tokens[0].pop_back();
		_RequestMap[tokens[0]] = std::vector<std::string>(tokens.begin() + 1, tokens.end());
	}
}

size_t Request::getHeaderLength()
{
	size_t pos = -1;
	if ((pos = _req_header.find("\r\n\r\n")) != std::string::npos)
		return pos;
	else
		return -1;
}

void Request::setContentType()
{
	if (_req_header.find("Content-Type: ") != std::string::npos)
		this->_content_type = true;
	else
		this->_content_type = false;
}

long long Request::getContentLength()
{
	size_t pos;
	long long length = 0;
	char *ptr_end;

	if (_content_length == -1 && (pos = _req_header.find("Content-Length: ")) != std::string::npos) // if content-Length is found
	{
		length = std::strtoll(&_req_header[pos + 16], &ptr_end, 10) + 4;
		this->_transfer_encoding = COMPLETED;
	}
	else if (_content_length == -1 && (_req_header.find("Transfer-Encoding: chunked")) != std::string::npos) // !content-Lenght && transfer-Encoding = chunked
		this->_transfer_encoding = CHUNKED;
	else // Content-Length not found && !chunked
		_transfer_encoding = COMPLETED;
	return length;
}

std::pair<std::string, std::string> &Request::getQueriesScriptName()
{
	return _url_queries_scriptName;
}

const int &Request::getBodyFD() const
{
	return _fd;
}

bool Request::isBadRequest()
{
	return _bad_request_found;
}

void Request::_checkForBadRequest()
{
	// case1: The host is not provided.
	if (!_RequestMap.count("Host"))
		_bad_request_found = true;
}

std::string Request::generateFilename(int fd)
{
	time_t seconds;

	seconds = time(NULL);
	return "/tmp/req_" + std::to_string(fd) + "_" + std::to_string(seconds);
}

const bool &Request::getIsFobiddenMethod() const
{
	return _is_forbiden_method;
}

bool _is_allowed_method(std::string const &http_method)
{
	return (http_method == "GET" || http_method == "POST" || http_method == "DELETE");
}