#pragma once
# include <iostream>
# include <fstream>
# include <algorithm>
# include "location.hpp"

class Response
{
	public:
		Response(void);
		Response(Response const&);
		Response& operator=(Response const&);
		void Get_request(std::string const&, Location const&, std::string const&);
		std::string const& 	get_response(void) const;
		size_t				get_error(void)	 const;
	private:
		void _default_location(std::string const&, Location const&, std::string const&);
		std::string _response;
		size_t		_error;
};