#include "../includes/response.hpp"
#include "../includes/location.hpp"


Response::Response(void): _response(""), _error(0){}
Response::Response(Response const& x) { *this = x;	}
Response& Response::operator=(Response const& x)
{
	_response = x._response;
	_error = x._error;
	return *this;
}
void Response::Get_request(std::string const& root, Location const& loc, std::string const& uri)
{
	/*
	* if the location is the @default location then we should check the type of the uri, is it a directory or a file
	* and if it's a direcotry then we should check if it containes one of the names in the index directive, then we should return
	* it's content, otherwise we should check if the autoindex it set to on means that we should list all the files in that directory
	* if non of the prev condition is true then we should return an error
	*/
	if (loc.getPath() == "/")
		_default_location(root, loc, uri);
}

void Response::_default_location(std::string const& root, Location const& loc, std::string const& uri)
{
	std::string const				full_path(root + '/' + uri);
	std::vector<std::string> const	index = loc.getIndex();
	std::ifstream					file;
	std::string 					line;
	std::string						tmp_resp;
	size_t							content_counter(0);

	// first lets search if the location contains the uri as one of its index names
	if (uri.empty())
	{
		for (size_t i = 0; i < index.size(); ++i)
		{
			file.open(root + '/' + index[i]);
			if (file.is_open())
				break;
		}
	}
	else
		file.open(full_path);
	// if the file is good we will start reading its content
	if (!file.is_open() || !file.good())
	{
		_error = 404;
		return;
	}
	while (!file.eof())
	{
		std::getline(file, line);
		content_counter += line.size();
		if (!file.eof())
		{
			line += '\n';
			content_counter++;
		}
		tmp_resp += line;
	}
	_set_headers(200, "OK", content_counter);
	_response += tmp_resp;
	file.close();
}

void Response::_set_headers(size_t status_code, std::string const& message, size_t content_length)
{
	time_t rawtime;

	time (&rawtime);
	_response += "HTTP/1.1 " +  std::to_string(status_code) + " " + message + '\n';
	_response += "Date: " + std::string(ctime(&rawtime));
	_response += "Server: webserver\n";
	_response += "Content-Length: " + std::to_string(content_length) + '\n';
	_response += "Content-Type: \n";
	_response += "Connection: close\n";
	_response += '\n';
}

std::string const& 	Response::get_response(void) const	{ return _response; }
size_t				Response::get_error(void)	 const	{ return _error; 	}