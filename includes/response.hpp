#pragma once
# include <iostream>
# include <fstream>
# include <algorithm>
# include <ctime>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <map>
# include "location.hpp"

class Response
{
	public:
		Response(void);
		Response(std::string const&, Location const&, std::string const&, std::string const&);
		Response(Response const&);
		Response& operator=(Response const&);
		~Response(void);
	public:
		void 				Get_request(void);
		std::string const& 	get_response(void) const;
	private:
		bool 	_default_location(void);
		void 	_set_headers(size_t, std::string const&, size_t, std::string const&);
		void	_fill_response(std::string const&, size_t, std::string const&);
		bool	_file_is_good(void);
	private:
		std::string							_response;
		std::ifstream						_file;
		std::string 						_file_path;
		Location							_loc;
		std::string	const 					_root;
		std::string	const					_uri;
		std::string const 					_error_pages;
		std::map<std::string, std::string> 	_type;
};