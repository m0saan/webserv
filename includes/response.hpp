#pragma once
# include <iostream>
# include <fstream>
# include <algorithm>
# include <ctime>
# include <cstring>
# include <unistd.h>
# include <cstdlib>
# include <fcntl.h>
# include <sys/stat.h>
# include <sstream>
# include <errno.h>
# include <map>
# include <vector>
# include <dirent.h>
# include "location.hpp"
# include "../parser/parser.hpp"

class Response
{
	public:
		Response(ServerConfig & config, std::map<std::string, std::vector<std::string> >& request_map, std::pair<std::string, std::string>& queries_script_name);
		Response(Response const&);
		Response& operator=(Response const&);
		~Response(void);
	public:
		void 				Get_request(void);
		void				Redirection(void);
		void 				Post_request(void);
		void				Delete_request(void);
		void				bad_allocation(void);
		void				internal_error(void);
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
		void	_fill_cgi_response(std::string *, bool);
		void	_auto_index_list(void);
		void	_fill_auto_index_response(std::string *);
		void	_fill_status_codes(void);
		void	_redirect_with_location(size_t);
		void	_redirect_without_location(size_t);
		std::vector<char const*>	_cgi_meta_var(void);

	private:
		std::string							_response;
		std::ifstream						_file;
		std::string 						_file_path;
		std::string							_root;
		std::string							_uri;
		std::map<std::string, std::string>& _error_pages;
		std::map<std::string, std::string> 	_type;
		ServerConfig&						_server_configs;
		std::map<std::string, std::vector<std::string> >& _request_map;
		std::pair<std::string, std::string>& _queries_script_name;
		std::map<std::string, std::string>*	_status_codes;
};