/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbani <mbani@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/09 10:09:08 by mbani             #+#    #+#             */
/*   Updated: 2021/11/14 17:06:49 by mbani            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>

typedef enum type {
	GET, POST, DELETE
} type;

typedef enum transfer_type
{
	CHUNKED, COMPLETED
} transfer_type;

class request
{
	private:
		std::string		_req;
		long long 		_size;
		long long 		_content_length;
		long long		_header_length;
		long long		_max_body_size;
		// type			method;
		transfer_type	transfer_encoding;
	public:
		request(long long max_size = 100000);
		// request(char *content, long long lenght, long long content_length);
		bool is_completed() const;
		void append(char *content, long long size);
		std::string get_req()const;
		void getReqInfo(std::string );
		long long getContentLength(std::string str);
		size_t getHeaderLength(std::string str);
		~request();
};