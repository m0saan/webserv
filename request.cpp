/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbani <mbani@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/09 10:15:00 by mbani             #+#    #+#             */
/*   Updated: 2021/11/14 18:21:07 by mbani            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"


request::request(long long max_size ):_size(-1), _content_length(-1), _header_length(-1), _max_body_size(max_size)
{
}

// request::request(char *content, long long lenght, long long content_length):_req(std::string(content, lenght)), _size(lenght), _content_length(content_length)
// {
// }


std::string request::get_req()const
{
	return this->_req;
}

bool	request::is_completed() const
{
	return (_size == _content_length + _header_length);
}

void 	request::append(char *content, long long size)
{
	std::string tmp(content, size);
	if (_content_length == -1) // 1st time reading req
		getReqInfo(std::string(content, size));
	_req += tmp;
	_size = _req.length();
}

void request::getReqInfo(std::string str)
{
	_content_length = getContentLength(str);
	_header_length = getHeaderLength(str);
}

size_t request::getHeaderLength(std::string str)
{
	size_t pos = 0;

	if ((pos = str.find("\r\n\r\n")) != std::string::npos)
		_header_length = (pos + 5);
	else
		throw std::exception();
	return pos;
}

long long request::getContentLength(std::string str)
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

			else maybe it's a chunked req or bad request

	*/
	
	return length;
}
request::~request()
{
}