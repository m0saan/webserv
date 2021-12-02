#pragma once
# include <iostream>
# include <fstream>
# include <algorithm>
# include <ctime>
# include <cstring>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
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
		void 				Post_request(void);
		void				Delete_request(void);
		std::string const& 	get_response(void) const;
	private:
		void 	_set_headers(size_t, std::string const&, size_t, std::string const&);
		void	_fill_response(std::string const&, size_t, std::string const&);
		bool	_file_is_good(bool);
		bool	_is_dir(std::string const&) const;
		void	_process_as_dir(void);
		void	_process_as_file(void);
		void	_process_post_delete(std::string const&);
		void	_cgi(void);
		void	_fill_cgi_response(std::string const&, bool);
	private:
		std::string							_response;
		std::ifstream						_file;
		std::string 						_file_path;
		Location							_loc;
		std::string							_root;
		std::string							_uri;
		std::string 						_error_pages;
		std::map<std::string, std::string> 	_type;
};