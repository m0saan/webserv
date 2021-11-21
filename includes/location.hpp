#pragma once
#include <iostream>
#include <vector>

class Location
{
	public:
		Location(){}
		Location(std::string const& path, std::vector<std::string> const& index, std::vector<std::string> const& allowed): 
		_path(path), _index(index), _allowed_methods(allowed){}
		std::string const& getPath(void) const { return _path; }
		std::vector<std::string> getIndex(void) const { return _index; }
		std::vector<std::string> getAllowedMethods(void) const { return _allowed_methods; }
		std::string	const& getAutoIndex(void) const { return _autoIndex;}
	private:
		std::string 				_path;
		std::vector<std::string> 	_index;
		std::vector<std::string>	_allowed_methods;
		std::string 				_autoIndex;
		std::string					_cgi_path;
};