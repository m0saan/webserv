#pragma once
#include <iostream>
#include <vector>

class Location
{
	public:
		Location(){}
		Location(std::string const& path, std::vector<std::string> const& index): _path(path), _index(index){}
		std::string const& getPath(void) const { return _path; }
		std::vector<std::string> getIndex(void) const { return _index; }
		std::string	const& getAutoIndex(void) const { return _autoIndex;}
	private:
		std::string 				_path;
		std::vector<std::string> 	_index;
		std::string 				_autoIndex;
};