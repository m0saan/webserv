#include "../includes/response.hpp"
#include "../includes/location.hpp"


Response::Response(void): _response(""), _loc()
, _root(""), _uri(""), _error_pages(""){}

Response::Response(std::string const& root, Location const& loc
, std::string const& uri, std::string const& error_pages): _loc(loc), _root(root)
, _uri(uri), _error_pages(error_pages)
{
	_type.insert(std::make_pair("json", "application"));
	_type.insert(std::make_pair("html", "text"));
	_type.insert(std::make_pair("php", "application/octet-stream"));
}

Response::Response(Response const& x) { *this = x;	}
Response::~Response(void) { _file.close(); }
Response& Response::operator=(Response const& x)
{
	_response = x._response;
	return *this;
}

void Response::Delete_request(void)	{	_process_post_delete("DELETE");	}

void Response::Post_request(void)	{	_process_post_delete("POST");	}

void Response::Get_request(void)
{
	std::vector<std::string> 	allowed = _loc.getAllowedMethods();
	std::string const 			loc_path = _loc.getPath();

	// lets first check for alowed methods in this location
	if (find(allowed.begin(), allowed.end(), "GET") == allowed.end())
	{
		_fill_response(_error_pages + '/' + "403.html", 403, "Forbiden");
		return;
	}
	// now lets check if we have to pass the file to the cgi (when we have a .php location), or process it as a static file otherwise
	if (_uri.substr(_uri.find_last_of(".") + 1) == "php" && loc_path.substr(loc_path.find_last_of(".") + 1) == "php")
	{
		std::cout << "we have to call the cgi" << std::endl;
		return;
	}
	// first we have to check if the location is a dir or just a file
	if (loc_path == "/")
		_process_as_dir();
	else if (_is_dir(_root + '/' + _loc.getPath()))
		_process_as_dir();
	else	
		_process_as_file();
}

void Response::_set_cgi_meta_var(void) 
{
	/*
	 * SRC = Request (we will get this info from the request headers)
	 * The server MUST set this meta-variable if and only if the request is accompanied by a message body entity.
	 *	The CONTENT_LENGTH value must reflect the length of the message-body
	 */
	_cgi_meta_var += "CONTENT_LENGHT=" + '\n';
	/*
	 * SRC = Request (we will get this info from the request headers)
	 * The server MUST set this meta-variable if an HTTP Content-Type field is present in the client request header.
	 */
	_cgi_meta_var += "CONTENT_TYPE=" + '\n';
	/*
	 * SRC = Static (hard code it)
	 * It MUST be set to the dialect of CGI being used by the server to communicate with the script. Example: CGI/1.1
	 */
	_cgi_meta_var += "GATEWAY_INTERFACE=CGI/1.1\n";
	/*
	 * SRC = Request (we will get this info from the request headers)
	 * Extra "path" information. It's possible to pass extra info to your script in the URL,
	 * after the filename of the CGI script. For example, calling the 
	 * URL http://www.myhost.com/mypath/myscript.cgi/path/info/here will set PATH_INFO to "/path/info/here".
	 * Commonly used for path-like data, but you can use it for anything.
	 */
	_cgi_meta_var += "PATH_INFO=" + '\n';
	/*
	 * SRC = Request/Conf (we will get this info from the request headers but we should parse it as a local uri)
	 * he PATH_TRANSLATED variable is derived by taking the PATH_INFO value, parsing it as a local URI in its own right,
	 * and performing any virtual-to-physical translation appropriate to map it onto the server's document repository structure
	 */
	_cgi_meta_var += "PATH_TRANSLATED=" + '\n';
	/*
	 * SRC = Request
	 * When information is sent using a method of GET, this variable contains the information in a query that follows the "?".
	 * The string is coded in the standard URL format of changing spaces to "+" and encoding special characters with "%xx" hexadecimal encoding.
	 * The CGI program must decode this information.
	 */
	_cgi_meta_var += "QUERY_STRING=" + '\n';
	/*
	 * SRC = Request
	 * Contains the method (as specified with the METHOD attribute in an HTML form) that is
	 * used to send the request. Example: GET
	 */
	_cgi_meta_var += "REQUEST_METHOD=" + '\n';
	/*
	 * SRC = Request
	 * The path part of the URL that points to the script being executed.
	 * It should include the leading slash. Example: /cgi-bin/hello.pgm
	 */
	_cgi_meta_var += "SCRIPT_NAME=" + '\n';
	/* 
	 * SRC = Conf
	 * Contains the server host name or IP address of the server. Example: 10.9.8.7
	 */
	_cgi_meta_var += "SERVER_NAME=" + '\n';
	/*
	 * Contains the port number to which the client request was sent.
	 */
	_cgi_meta_var += "SERVER_PORT=" + '\n';
	_cgi_meta_var += "SERVER_PROTOCOL=HTTP/1.1\n";
	_cgi_meta_var += "SERVER_SOFTWARE=Webserv\n";
}
void Response::_cgi(void)
{
	// lets first set the cgi meta-variables
}

void Response::_process_post_delete(std::string const& req_method)
{
	std::vector<std::string> 	const	allowed = _loc.getAllowedMethods();
	std::vector<std::string>	const 	index = _loc.getIndex();
	std::string const 					loc_path = _loc.getPath();
	bool								found(false);

	// lets first check for alowed methods in this location
	if (find(allowed.begin(), allowed.end(), req_method) == allowed.end())
	{
		_fill_response(_error_pages + '/' + "403.html", 403, "Forbiden");
		return;
	}
	// now lets check if we have to pass the file to the cgi (when we have a .php location), or process it as a static file otherwise
	if (_uri.substr(_uri.find_last_of(".") + 1) == "php" && loc_path.substr(loc_path.find_last_of(".") + 1) == "php")
	{
		std::cout << "we have to call the cgi" << std::endl;
		return;
	}
	// otherwise if the request method is delete then we should return a Not Allowed message
	if (req_method == "DELETE")
	{
		_fill_response(_error_pages + '/' + "405.html", 405, "Not Allowed");
		return;
	}
	// now if we have an other file extension than php, then we should return an error
	if (!_is_dir(_root + '/' + _uri))	
	{
		if (!_file_is_good(true)) // if the file doesn't exist then we should return a not found message
			return;
		_fill_response(_error_pages + '/' + "405.html", 405, "Not Allowed");
		return;
	}
	else // if the _uri is a dir, then it behavios as get request, if the file not found we should return a 403 error
	{
		_root += '/' + _uri;
		for (size_t i = 0; i < index.size(); ++i)
		{
			_file_path = _root + '/' + index[i];
			if (!(found = _file_is_good(false)) && errno != 2) // if the file exists but we don't have the permissions to read from it
			{
				_fill_response(_error_pages + '/' + "403.html", 403, "Forbiden");
				return;
			}
			else if (found)
			{
				if (loc_path == "/")
				{
					_fill_response(_error_pages + '/' + "405.html", 405, "Not Allowed");
					return;
				}
				_fill_response(_file_path, 200, "OK");
				break;
			}
		}
		if (!found && _loc.getAutoIndex() != "on")
		{
			_fill_response(_error_pages + '/' + "403.html", 403, "Forbiden");
			return;
		}	
	}
	// if we are here then we have a dir in the _uri, and the auto index is set to on, so we should list all the files in the dir
}
void Response::_process_as_dir(void)
{
	std::vector<std::string> const	index = _loc.getIndex();
	bool							found(false);
	
	_root += '/' + _uri;
	if (_uri.empty() || _is_dir(_root))
	{
		for (size_t i = 0; i < index.size(); ++i)
		{
			_file_path = _root + '/' + index[i];
			if (!(found = _file_is_good(false)) && errno != 2) // if the file exists but we don't have the permissions to read from it
			{
				_fill_response(_error_pages + '/' + "403.html", 403, "Forbiden");
				return;
			}
			else if (found)
			{
				if (!_file_is_good(true))
					return;
				_fill_response(_file_path, 200, "OK");
				break;
			}
		}
		if (!found && _loc.getAutoIndex() != "on")
		{
			_fill_response(_error_pages + '/' + "404.html", 404, "Not found");
			return;
		}	
	}
	else
	{
		_file_path = _root;
		if (!_file_is_good(true))
			return;
		_fill_response(_file_path, 200, "OK");
		return;
	}
	// if we are here than we didn't found the file we are seaching on, and we have a intoindex set to on, so we should fill the template for 
	// autoindex on to list all the files in the dir
}

void Response::_process_as_file(void)
{
	_file_path = _root + '/' + _uri;
	if (!_file_is_good(true))
		return;
	_fill_response(_file_path, 200, "OK");
	return;
}

bool Response::_is_dir(std::string const& path) const
{
	struct stat s;

	if (!lstat(path.c_str(), &s))
	{
		if (S_ISDIR(s.st_mode))
			return true;
		else
			return false;
	}
	return false;
}

void Response::_set_headers(size_t status_code, std::string const& message, size_t content_length, std::string const& path)
{
	time_t rawtime;
	std::string const extention = path.substr(path.find_last_of(".") + 1); 

	time (&rawtime);
	_response += "HTTP/1.1 " +  std::to_string(status_code) + " " + message + '\n';
	_response += "Date: " + std::string(ctime(&rawtime));
	_response += "Server: webserver\n";
	_response += "Content-Length: " + std::to_string(content_length) + '\n';
	if (extention == "php")
		_response += "Content-Type: " + _type[extention] + '\n';
	else
		_response += "Content-Type: " + _type[extention] + '/' + extention + '\n';
	_response += "Connection: close\n";
	_response += '\n';
}

void Response::_fill_response(std::string const& path, size_t status_code, std::string const& message)
{
	std::string 	line;
	std::string		tmp_resp;
	size_t			content_counter(0);
	
	_file.open(path);
	while (!_file.eof())
	{
		std::getline(_file, line);
		content_counter += line.size();
		if (!_file.eof())
		{
			line += '\n';
			content_counter++;
		}
		tmp_resp += line;
	}
	// set all the needed response header
	_set_headers(status_code, message, content_counter, path);
	_response += tmp_resp;
}

bool Response::_file_is_good(bool fill_resp)
{
	if (_file_path.empty())
		_file_path = _root + '/' + _uri;
	if (open(_file_path.c_str(), O_RDONLY) < 0)
	{
		if (errno == 2 && fill_resp)
			_fill_response(_error_pages + '/' + "404.html", 404, "Not Found");
		else if (fill_resp)
			_fill_response(_error_pages + '/' + "403.html", 403, "Forbidden");
		return false;
	}
	return true;
}
std::string const& 	Response::get_response(void) const	{ return _response; }