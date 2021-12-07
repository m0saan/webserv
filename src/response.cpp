#include "../includes/response.hpp"
#include "../includes/location.hpp"

/* this is the implementation of the default, param, and copy constructors plus the operator=*/

Response::Response(ServerConfig & config, std::map<std::string, std::vector<std::string> >& request_map, std::pair<std::string, std::string>& queries_script_name)
:
_server_configs(config),
_request_map(request_map),
_queries_script_name(queries_script_name)

{
	_type.insert(std::make_pair("json", "application"));
	_type.insert(std::make_pair("html", "text"));
	_type.insert(std::make_pair("php", "application/octet-stream"));
	_uri = _request_map["ST"][1];
	if (*_uri.begin() == '/')
		_uri.erase(_uri.begin());
	_root = _server_configs._root;
	// _error_pages = _server_configs._error_page;
	_fill_status_codes();
}

Response::Response(Response const& x)
: _server_configs(x._server_configs),
_request_map(x._request_map),
_queries_script_name(x._queries_script_name) { *this = x;	}

Response::~Response(void) { _file.close(); delete _status_codes; }
Response& Response::operator=(Response const& x)
{
	_response = x._response;
	return *this;
}

void Response::_fill_status_codes(void)
{
	_status_codes = new std::map<std::string, std::string>();
	/*----------------- 1xx status codes -------------*/
	// 	< HTTP/1.1 100 
	// < Server: nginx/1.21.4
	// < Date: Tue, 07 Dec 2021 10:55:30 GMT
	// < Content-Type: application/octet-stream
	// < Content-Length: 17
	// < Connection: keep-alive
	// * Excess found in a non pipelined read: excess = 17, size = 17, maxdownload = 17, bytecount = 17
	// * Connection #0 to host localhost left intact
	// http://google.com
	_status_codes->insert(std::make_pair("100", ""));
	_status_codes->insert(std::make_pair("101", ""));
	_status_codes->insert(std::make_pair("102", ""));
	_status_codes->insert(std::make_pair("103", ""));
	/*-----------------------------------------------*/
	/*----------------- 2xx status codes -------------*/
	// 	HTTP/1.1 200 OK
	// < Server: nginx/1.21.4
	// < Date: Mon, 06 Dec 2021 02:52:37 GMT
	// < Content-Type: application/octet-stream
	// < Content-Length: 17
	// < Connection: keep-alive
	// < 
	// * Connection #0 to host localhost left intact
	// http://google.com
	_status_codes->insert(std::make_pair("200", "OK"));
	// 	HTTP/1.1 201 Created
	_status_codes->insert(std::make_pair("201", "Created"));
	// < HTTP/1.1 202 Accepted
	_status_codes->insert(std::make_pair("202", "Accepted"));
	// < HTTP/1.1 203 
	_status_codes->insert(std::make_pair("203", ""));
	// 	< HTTP/1.1 204 No Content
	// < Server: nginx/1.21.4
	// < Date: Mon, 06 Dec 2021 02:55:50 GMT
	// < Connection: keep-alive
	_status_codes->insert(std::make_pair("204", "No Content"));
	// < HTTP/1.1 205 
	_status_codes->insert(std::make_pair("205", ""));
	// < HTTP/1.1 206 Partial Content
	_status_codes->insert(std::make_pair("206", "Partial Content"));
	// < HTTP/1.1 207 
	_status_codes->insert(std::make_pair("207", ""));
	// < HTTP/1.1 208 
	_status_codes->insert(std::make_pair("208", ""));
	// < HTTP/1.1 226 
	_status_codes->insert(std::make_pair("226", ""));
	/*-----------------------------------------------*/
	/*----------------- 3xx status codes -------------*/
	// 	< HTTP/1.1 300 
	// < Server: nginx/1.21.4
	// < Date: Tue, 07 Dec 2021 10:45:07 GMT
	// < Content-Type: application/octet-stream
	// < Content-Length: 17
	// < Connection: keep-alive
	// < 
	// * Connection #0 to host localhost left intact
	// http://google.com
	_status_codes->insert(std::make_pair("300", ""));
	// 	< HTTP/1.1 301 Moved Permanently
	// < Server: nginx/1.21.4
	// < Date: Tue, 07 Dec 2021 10:45:43 GMT
	// < Content-Type: text/html
	// < Content-Length: 169
	// < Connection: keep-alive
	// < Location: http://google.com
	// < 
	// <html>
	// <head><title>301 Moved Permanently</title></head>
	// <body>
	// <center><h1>301 Moved Permanently</h1></center>
	// <hr><center>nginx/1.21.4</center>
	// </body>
	// </html>
	_status_codes->insert(std::make_pair("301", ""));
	// 	< HTTP/1.1 302 Moved Temporarily
	// < Server: nginx/1.21.4
	// < Date: Tue, 07 Dec 2021 10:46:11 GMT
	// < Content-Type: text/html
	// < Content-Length: 145
	// < Connection: keep-alive
	// < Location: http://google.com
	// < 
	// <html>
	// <head><title>302 Found</title></head>
	// <body>
	// <center><h1>302 Found</h1></center>
	// <hr><center>nginx/1.21.4</center>
	// </body>
	// </html>
	_status_codes->insert(std::make_pair("302", ""));
	// 	< HTTP/1.1 303 See Other
	// < Server: nginx/1.21.4
	// < Date: Tue, 07 Dec 2021 10:46:50 GMT
	// < Content-Type: text/html
	// < Content-Length: 153
	// < Connection: keep-alive
	// < Location: http://google.com
	// < 
	// <html>
	// <head><title>303 See Other</title></head>
	// <body>
	// <center><h1>303 See Other</h1></center>
	// <hr><center>nginx/1.21.4</center>
	// </body>
	// </html>
	_status_codes->insert(std::make_pair("303", ""));
	// 	< HTTP/1.1 304 Not Modified
	// < Server: nginx/1.21.4
	// < Date: Tue, 07 Dec 2021 10:47:14 GMT
	// < Content-Type: application/octet-stream
	// < Content-Length: 17
	// < Connection: keep-alive
	_status_codes->insert(std::make_pair("304", ""));
	// 	< HTTP/1.1 305 
	// < Server: nginx/1.21.4
	// < Date: Tue, 07 Dec 2021 10:47:40 GMT
	// < Content-Type: application/octet-stream
	// < Content-Length: 17
	// < Connection: keep-alive
	// < 
	// * Connection #0 to host localhost left intact
	// http://google.com
	_status_codes->insert(std::make_pair("305", ""));
	// 	< HTTP/1.1 306 
	// < Server: nginx/1.21.4
	// < Date: Tue, 07 Dec 2021 10:47:59 GMT
	// < Content-Type: application/octet-stream
	// < Content-Length: 17
	// < Connection: keep-alive
	// < 
	// * Connection #0 to host localhost left intact
	// http://google.com%
	_status_codes->insert(std::make_pair("306", ""));
	// 	< HTTP/1.1 307 Temporary Redirect
	// < Server: nginx/1.21.4
	// < Date: Tue, 07 Dec 2021 10:48:22 GMT
	// < Content-Type: text/html
	// < Content-Length: 171
	// < Connection: keep-alive
	// < Location: http://google.com
	// < 
	// <html>
	// <head><title>307 Temporary Redirect</title></head>
	// <body>
	// <center><h1>307 Temporary Redirect</h1></center>
	// <hr><center>nginx/1.21.4</center>
	// </body>
	// </html>
	_status_codes->insert(std::make_pair("307", ""));
	// 	< HTTP/1.1 308 Permanent Redirect
	// < Server: nginx/1.21.4
	// < Date: Tue, 07 Dec 2021 10:48:46 GMT
	// < Content-Type: text/html
	// < Content-Length: 171
	// < Connection: keep-alive
	// < Location: http://google.com
	// < 
	// <html>
	// <head><title>308 Permanent Redirect</title></head>
	// <body>
	// <center><h1>308 Permanent Redirect</h1></center>
	// <hr><center>nginx/1.21.4</center>
	// </body>
	// </html>
	_status_codes->insert(std::make_pair("308", ""));
	/*-----------------------------------------------*/
	/*----------------- 4xx status codes -------------*/
	// 	 HTTP/1.1 400 Bad Request
	// < Server: nginx/1.21.4
	// < Date: Mon, 06 Dec 2021 03:08:13 GMT
	// < Content-Type: application/octet-stream
	// < Content-Length: 17
	// < Connection: keep-alive
	// < 
	// * Connection #0 to host localhost left intact
	// http://google.com%
	_status_codes->insert(std::make_pair("400", "Bad Request"));
	// < HTTP/1.1 401 Unauthorized
	_status_codes->insert(std::make_pair("401", "Unauthorized"));
	// < HTTP/1.1 402 Payment Required
	_status_codes->insert(std::make_pair("402", "Payment Required"));
	// < HTTP/1.1 403 Forbidden
	_status_codes->insert(std::make_pair("403", "Forbidden"));
	// < HTTP/1.1 404 Not Found
	_status_codes->insert(std::make_pair("404", "Not Found"));
	// < HTTP/1.1 405 Not Allowed
	_status_codes->insert(std::make_pair("405", "Not Allowed"));
	// < HTTP/1.1 406 Not Acceptable
	_status_codes->insert(std::make_pair("406", "Not Acceptable"));
	// < HTTP/1.1 407 
	_status_codes->insert(std::make_pair("407", ""));
	// < HTTP/1.1 408 Request Time-out
	_status_codes->insert(std::make_pair("408", "Request Time-out"));
	// < HTTP/1.1 409 Conflict
	_status_codes->insert(std::make_pair("409", "Conflict"));
	// < HTTP/1.1 410 Gone
	_status_codes->insert(std::make_pair("410", "Gone"));
	// < HTTP/1.1 411 Length Required
	_status_codes->insert(std::make_pair("411", "Length Required"));
	// < HTTP/1.1 412 Precondition Failed
	_status_codes->insert(std::make_pair("412", "Precondition Failed"));
	// < HTTP/1.1 413 Request Entity Too Large
	_status_codes->insert(std::make_pair("413", "Request Entity Too Large"));
	// < HTTP/1.1 414 Request-URI Too Large
	_status_codes->insert(std::make_pair("414", "Request-URI Too Large"));
	// < HTTP/1.1 415 Unsupported Media Type
	_status_codes->insert(std::make_pair("415", "Unsupported Media Type"));
	// < HTTP/1.1 416 Requested Range Not Satisfiable
	_status_codes->insert(std::make_pair("416", "Requested Range Not Satisfiable"));
	// < HTTP/1.1 417 
	_status_codes->insert(std::make_pair("417", ""));
	// < HTTP/1.1 418 
	_status_codes->insert(std::make_pair("418", ""));
	// < HTTP/1.1 421 Misdirected Request
	_status_codes->insert(std::make_pair("421", "Misdirected Request"));
	// < HTTP/1.1 422 
	_status_codes->insert(std::make_pair("422", ""));
	// < HTTP/1.1 423 
	_status_codes->insert(std::make_pair("423", ""));
	// < HTTP/1.1 424 
	_status_codes->insert(std::make_pair("424", ""));
	// < HTTP/1.1 426 
	_status_codes->insert(std::make_pair("426", ""));
	// < HTTP/1.1 428 
	_status_codes->insert(std::make_pair("428", ""));
	// < HTTP/1.1 429 Too Many Requests
	_status_codes->insert(std::make_pair("429", "Too Many Requests"));
	// < HTTP/1.1 431 
	_status_codes->insert(std::make_pair("431", ""));
	// < HTTP/1.1 444 
	_status_codes->insert(std::make_pair("444", ""));
	// < HTTP/1.1 451 
	_status_codes->insert(std::make_pair("451", ""));
	// < HTTP/1.1 499 
	_status_codes->insert(std::make_pair("499", ""));
	/*-----------------------------------------------*/
	/*----------------- 5xx status codes -------------*/
	// 	< HTTP/1.1 500 Internal Server Error
	// < Server: nginx/1.21.4
	// < Date: Mon, 06 Dec 2021 03:42:54 GMT
	// < Content-Type: application/octet-stream
	// < Content-Length: 17
	// < Connection: keep-alive
	// < 
	// * Connection #0 to host localhost left intact
	// http://google.com%
	_status_codes->insert(std::make_pair("500", "Internal Server Error"));
	// < HTTP/1.1 501 Not Implemented
	_status_codes->insert(std::make_pair("501", "Not Implemented"));
	// < HTTP/1.1 502 Bad Gateway
	_status_codes->insert(std::make_pair("502", "Bad Gateway"));
	// < HTTP/1.1 503 Service Temporarily Unavailable
	_status_codes->insert(std::make_pair("503", "Service Temporarily Unavailable"));
	// < HTTP/1.1 504 Gateway Time-out
	_status_codes->insert(std::make_pair("504", "Gateway Time-out"));
	// < HTTP/1.1 505 HTTP Version Not Supported
	_status_codes->insert(std::make_pair("505", "HTTP Version Not Supported"));
	// < HTTP/1.1 506 
	_status_codes->insert(std::make_pair("506", ""));
	// < HTTP/1.1 507 Insufficient Storage
	_status_codes->insert(std::make_pair("507", "Insufficient Storage"));
	// < HTTP/1.1 508 
	_status_codes->insert(std::make_pair("508", ""));
	// < HTTP/1.1 510 
	_status_codes->insert(std::make_pair("510", ""));
	// < HTTP/1.1 511 
	_status_codes->insert(std::make_pair("511", ""));
	// < HTTP/1.1 599 
	_status_codes->insert(std::make_pair("599", ""));
	/*------------------------------------------------*/

}

/*-------------------------------------------------------------------------------*/

/* those are the public methods to process the request @GET @POST @DELETE  and Redirection*/

// this is the response for redirection with location header field
// 	< HTTP/1.1 301 Moved Permanently
// < Server: nginx/1.21.4
// < Date: Tue, 07 Dec 2021 10:45:43 GMT
// < Content-Type: text/html
// < Content-Length: 169
// < Connection: keep-alive
// < Location: http://google.com
// < 
// <html>
// <head><title>301 Moved Permanently</title></head>
// <body>
// <center><h1>301 Moved Permanently</h1></center>
// <hr><center>nginx/1.21.4</center>
// </body>
// </html>
void Response::_redirect_with_location(size_t status_code)
{
	time_t 		rawtime;
	std::string	*tmp_res;
	std::string status = _server_configs._location[0]._redirect.first;
	std::string	message =	_status_codes->operator[](status);
	std::stringstream ss;

	time (&rawtime);
	if (status_code != 302)
		tmp_res = error_page(status + ' ' + message);
	else
		tmp_res = error_page(status + ' ' + "302 Found");
	ss << tmp_res->length();
	_response += "HTTP/1.1 " + status + ' ' + message + "\r\n";
	_response += "Date: " + std::string(ctime(&rawtime));
	_response.erase(--_response.end());
	_response += "\r\nServer: webserver\r\n";
	_response += "Content-Type: text/html\r\n";
	_response += "Content-Length: " + ss.str() + "\r\n";
	_response += "Connection: keep-alive\r\n";
	_response += "Location: " + _server_configs._location[0]._redirect.second + "\r\n\r\n";
	_response += *tmp_res;
	delete tmp_res;
}
// this is the response for redirections without location header field
// 	< HTTP/1.1 500 Internal Server Error
// < Server: nginx/1.21.4
// < Date: Mon, 06 Dec 2021 03:42:54 GMT
// < Content-Type: application/octet-stream
// < Content-Length: 17
// < Connection: keep-alive
// < 
// * Connection #0 to host localhost left intact
// http://google.com%
/*---------------------------------------------*/
// redirect for 204 status_code
// < HTTP/1.1 204 No Content
// < Server: nginx/1.21.4
// < Date: Mon, 06 Dec 2021 02:55:50 GMT
// < Connection: keep-alive
/*---------------------------------------------*/
// 	< HTTP/1.1 304 Not Modified
// < Server: nginx/1.21.4
// < Date: Tue, 07 Dec 2021 10:47:14 GMT
// < Content-Type: application/octet-stream
// < Content-Length: 17
// < Connection: keep-alive
void Response::_redirect_without_location(size_t status_code)
{
	time_t 		rawtime;
	std::string	*tmp_res;
	std::string status = _server_configs._location[0]._redirect.first;
	std::string	message =	_status_codes->operator[](status);
	std::stringstream ss;

	time (&rawtime);
	_response += "HTTP/1.1 " + status + ' ' + message + "\r\n";
	_response += "Date: " + std::string(ctime(&rawtime));
	_response.erase(--_response.end());
	_response += "\r\nServer: webserver\r\n";
	_response += "Connection: keep-alive\r\n";
	if (status_code == 204)
		return;
	if (status_code != 304)
		tmp_res = error_page(status + ' ' + message);
	else
		*tmp_res =  _server_configs._location[0]._redirect.second + "\r\n";
	ss << tmp_res->length();
	_response += "Content-Type: application/octet-stream\r\n";
	_response += "Content-Length: " + ss.str() + "\r\n";
	if (status_code != 304)
	{
		_response += *tmp_res;
		delete tmp_res;
	}
}
void Response::Redirection(void)
{
	size_t				status_code;
	std::stringstream	ss;

	ss << _server_configs._location[0]._redirect.first;
	ss >> status_code;
	if ((status_code >= 301 && status_code <= 303)
	|| (status_code == 307 || status_code == 308))
		_redirect_with_location();
	else
		_redirect_without_location();
}

void Response::Delete_request(void)	{	_process_post_delete("DELETE");	}

void Response::Post_request(void)	{	_process_post_delete("POST");	}

void Response::Get_request(void)
{
	std::vector<std::string> 	allowed(_server_configs._allowed_method.begin(), _server_configs._allowed_method.end());
	std::string const 			loc_path = _server_configs._location[0]._loc_path;

	// lets first check for alowed methods in this location
	// TODO: CHECK WHETHER ALLOWED METHODS ARE AVAILABLE;
	// TODO: CHECK WHICH ONE SHOULD BE USED.
	if (find(allowed.begin(), allowed.end(), "GET") == allowed.end())
	{
		_fill_response(".html", 403, "Forbiden");
		return;
	}
	// now lets check if we have to pass the file to the cgi (when we have a .php location), or process it as a static file otherwise
	if (_uri.substr(_uri.find_last_of(".") + 1) == "php" && loc_path.substr(loc_path.find_last_of(".") + 1) == "php")
	{
		_cgi();
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
/*----------------------------------------------------------------------------*/
/* this part is for the cgi (fill the meta_var and excute it) */
std::vector<char const*>	Response::_cgi_meta_var(void)
{
	std::vector<char const*> meta_var;
	std::string				str;

	/*
	 * SRC = Request (we will get this info from the request headers)
	 * The server MUST set this meta-variable if and only if the request is accompanied by a message body entity.
	 *	The CONTENT_LENGTH value must reflect the length of the message-body
	 */
	str = std::string("CONTENT_LENGHT=") + _request_map["Content-Length"][0] +  '\n';
	meta_var.push_back(str.c_str());
	// _cgi_meta_var += "CONTENT_LENGHT=" + '\n';
	/*
	 * SRC = Request (we will get this info from the request headers)
	 * The server MUST set this meta-variable if an HTTP Content-Type field is present in the client request header.
	 */
	str = std::string("CONTENT_TYPE=") + _request_map["Content-Type"][0] + '\n';
	meta_var.push_back(str.c_str());
	/*
	 * SRC = Static (hard code it)
	 * It MUST be set to the dialect of CGI being used by the server to communicate with the script. Example: CGI/1.1
	 */
	str = "GATEWAY_INTERFACE=CGI/1.1\n";
	meta_var.push_back(str.c_str());
	/*
	 * SRC = Request (we will get this info from the request headers)
	 * Extra "path" information. It's possible to pass extra info to your script in the URL,
	 * after the filename of the CGI script. For example, calling the
	 * URL http://www.myhost.com/mypath/myscript.cgi/path/info/here will set PATH_INFO to "/path/info/here".
	 * Commonly used for path-like data, but you can use it for anything.
	 */
	str = std::string("PATH_INFO=") + _queries_script_name.second + '\n';
	meta_var.push_back(str.c_str());
	/*
	 * SRC = Request/Conf (we will get this info from the request headers but we should parse it as a local uri)
	 * the PATH_TRANSLATED variable is derived by taking the PATH_INFO value, parsing it as a local URI in its own right,
	 * and performing any virtual-to-physical translation appropriate to map it onto the server's document repository structure
	 */
	str = std::string("PATH_TRANSLATED=") + '\n';
	meta_var.push_back(str.c_str());
	/*
	 * SRC = Request
	 * When information is sent using a method of GET, this variable contains the information in a query that follows the "?".
	 * The string is coded in the standard URL format of changing spaces to "+" and encoding special characters with "%xx" hexadecimal encoding.
	 * The CGI program must decode this information.
	 */
	str = std::string("QUERY_STRING=") + _queries_script_name.first + '\n';
	meta_var.push_back(str.c_str());
	/*
	 * SRC = Request
	 * Contains the method (as specified with the METHOD attribute in an HTML form) that is
	 * used to send the request. Example: GET
	 */
	str = std::string("REQUEST_METHOD=") + _request_map["ST"][0] + '\n';
	meta_var.push_back(str.c_str());
	/*
	 * SRC = Request
	 * The path part of the URL that points to the script being executed.
	 * It should include the leading slash. Example: /cgi-bin/hello.pgm
	 */
	str = std::string("SCRIPT_NAME=") + _queries_script_name.second + '\n';
	meta_var.push_back(str.c_str());
	/*
	 * SRC = Conf
	 * Contains the server host name or IP address of the server. Example: 10.9.8.7
	 */
	str = std::string("SERVER_NAME=") + _server_configs._host + '\n';
	meta_var.push_back(str.c_str());
	/*
	 * Contains the port number to which the client request was sent.
	 */
	str = std::string("SERVER_PORT=") + _server_configs._port + '\n';
	meta_var.push_back(str.c_str());
	str = "SERVER_PROTOCOL=HTTP/1.1\n";
	meta_var.push_back(str.c_str());
	str = "SERVER_SOFTWARE=Webserv\n";
	meta_var.push_back(str.c_str());
	meta_var.push_back(NULL);
	return meta_var;
}

std::string	*get_res(int fd)
{
	std::string *ans = new std::string();
	char		buff[1024];
	int 		ret;

	while ((ret = read(fd, buff, 1024)))
		*ans += buff;
	return ans;
}

void Response::_fill_cgi_response(std::string *tmp_res, bool is_red)
{
	time_t 				rawtime;

	time (&rawtime);
	if(is_red)
		_response += "HTTP/1.1 302 Found\r\n";
	else
		_response += "HTTP/1.1 200 OK\r\n";
	_response += "Date: " + std::string(ctime(&rawtime));
	_response.erase(--_response.end());
	_response += "\r\nServer: webserver\r\n";
	_response += "Transfer-Encoding: chunked\r\n";
	_response += "Connection: close\r\n\r\n";
	_response += *tmp_res;
}

void Response::_cgi(void)
{
	int fd = open("user_login.php", O_RDONLY);
	int			pfd[2];
	std::string	*tmp_res;
	size_t 		index;
	int			status;

	pipe(pfd);
	if(!(fork()))
	{
		std::vector<char const*> meta_var = _cgi_meta_var();
		std::vector<char const*> args;
		std::string path;

		args.push_back(_server_configs._location[0]._cgi.c_str());
		args.push_back(NULL);
		path = _server_configs._location[0]._cgi;
		close(pfd[0]);
		dup2(fd, 0);
		dup2(pfd[1], 1);
		if (execve(path.c_str(), const_cast<char *const*>(&(*args.begin()))
		, const_cast<char *const*>(&(*meta_var.begin()))) < 0)
		{
			close(pfd[1]);
			args.~vector();
			meta_var.~vector();
			exit(1);
		}
		exit(0);
	}
	wait(&status);
	close(pfd[1]);
	// first lets check if the cgi path is correct
	if (WEXITSTATUS(status) == 1)
	{
		close(pfd[0]);
		close(fd);
		_fill_response(".html", 502, "Bad Gateway");
		return;
	}
	tmp_res = get_res(pfd[0]);
	// if the tmp_res contains the Status header field then we should erase it, because it will be added
	if ((index = tmp_res->find("Status")) != std::string::npos)
		tmp_res->erase(tmp_res->begin() + index, tmp_res->begin() + tmp_res->find_first_of('\n', index) + 1);
	if (tmp_res->find("Location") != std::string::npos)
		_fill_cgi_response(tmp_res, true);
	else
		_fill_cgi_response(tmp_res, false);
	close(pfd[0]);
	close(fd);
	delete tmp_res;
}
/*---------------------------------------------------------------------------------------------------*/
/* this part is for the files and subdirectoreis listing e.i when we have the auto indexing */
void Response::_fill_auto_index_response(std::string* tmp_res)
{
	time_t 				rawtime;

	time (&rawtime);
	_response += "HTTP/1.1 200 OK\r\n";
	_response += "Date: " + std::string(ctime(&rawtime));
	_response.erase(--_response.end());
	_response += "\r\n";
	_response += "Server: webserver\r\n";
	_response += "Transfer-Encoding: chunked\r\n";
	_response += "Connection: close\r\n\r\n";
	_response += *tmp_res;
}

void Response::_auto_index_list(void)
{
	std::string 	*tmp_res = new std::string();
	DIR*			dir;
	struct dirent	*dir_info;

	dir = opendir(_root.c_str());
	if (!dir)
	{
		_fill_response(".html", 403, "Forbidden");
		return;
	}
	*tmp_res += std::string("<html>\r\n<head>\r\n<title>Index of " + _uri + "</title>\r\n");
	*tmp_res += "</head>\r\n<body>\r\n";
	*tmp_res += std::string("<h1>Index of " + _uri + "</h1>\r\n");
	*tmp_res += "<hr>\r\n<pre>\r\n";
	while ((dir_info = readdir(dir)))
	{
		if (std::string(dir_info->d_name) == ".")
			continue;
		if (std::string(dir_info->d_name) == "..")
		{
			*tmp_res += "<a href=\"";
			*tmp_res += std::string("../\">..</a>\r\n");
			continue;
		}
		*tmp_res += std::string("<a href=\"");
		*tmp_res += std::string(dir_info->d_name) + "\">" + std::string(dir_info->d_name) + "</a>\r\n";
	}
	*tmp_res += "</pre>\r\n<hr>\r\n</body>\r\n</html>\r\n";
	_fill_auto_index_response(tmp_res);
	delete tmp_res;
	closedir(dir);
}
/*-----------------------------------------------------------------------------------------------------*/
std::string	*error_page(std::string const& message)
{
	std::string *error_body = new std::string();

	*error_body += std::string("<html>\r\n<head>\r\n");
	*error_body += std::string("<title>") + message;
	*error_body += std::string("</title>\r\n</head>\r\n<body>\r\n<center>\r\n<h1>") + message;
	*error_body += std::string("</h1>\r\n</center>\r\n<hr>\r\n<center>webserver</center>\r\n</body>\r\n</html>\r\n");
	return error_body;
}
void Response::_process_post_delete(std::string const& req_method)
{
	std::vector<std::string> const 		allowed(_server_configs._allowed_method.begin(), _server_configs._allowed_method.end());
	std::string const 					loc_path = _server_configs._location[0]._loc_path;
	std::vector<std::string>	const 	index = _server_configs._location[0]._index;
	bool								found(false);

	// lets first check for alowed methods in this location
	if (find(allowed.begin(), allowed.end(), req_method) == allowed.end())
	{
		_fill_response(".html", 403, "Forbiden");
		return;
	}
	// now lets check if we have to pass the file to the cgi (when we have a .php location), or process it as a static file otherwise
	if (_uri.substr(_uri.find_last_of(".") + 1) == "php" && loc_path.substr(loc_path.find_last_of(".") + 1) == "php")
	{
		_cgi();
		return;
	}
	// otherwise if the request method is delete then we should return a Not Allowed message
	if (req_method == "DELETE")
	{
		_fill_response(".html", 405, "Not Allowed");
		return;
	}
	// now if we have an other file extension than php, then we should return an error
	if (!_is_dir(_root + '/' + _uri))
	{
		if (!_file_is_good(true)) // if the file doesn't exist then we should return a not found message
			return;
		_fill_response(".html", 405, "Not Allowed");
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
				_fill_response(".html", 403, "Forbiden");
				return;
			}
			else if (found)
			{
				if (loc_path == "/")
				{
					_fill_response(".html", 405, "Not Allowed");
					return;
				}
				_fill_response(_file_path, 200, "OK");
				return;
			}
		}
		if (!found && _loc.getAutoIndex() != "on")
		{
			_fill_response(".html", 403, "Forbiden");
			return;
		}
	}
	// if we are here then we have a dir in the _uri, and the auto index is set to on, so we should list all the files in the dir
	_auto_index_list();
}

void Response::_process_as_dir(void)
{
	std::vector<std::string>	const 	index = _server_configs._location[0]._index;
	bool								found(false);

	_root += '/' + _uri;
	if (_uri.empty() || _is_dir(_root))
	{
		for (size_t i = 0; i < index.size(); ++i)
		{
			_file_path = _root + '/' + index[i];
			if (!(found = _file_is_good(false)) && errno != 2) // if the file exists but we don't have the permissions to read from it
			{
				_fill_response(".html", 403, "Forbiden");
				return;
			}
			else if (found)
			{
				if (!_file_is_good(true))
					return;
				_fill_response(_file_path, 200, "OK");
				return;
			}
		}
		if (!found && _loc.getAutoIndex() != "on")
		{
			_fill_response(".html", 404, "Not found");
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
	_auto_index_list();
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
	std::stringstream ss, ss_content;

	ss << status_code;
	time (&rawtime);
	_response += "HTTP/1.1 " +  ss.str() + " " + message + "\r\n";
	_response += "Date: " + std::string(ctime(&rawtime));
	_response.erase(--_response.end());
	_response += "\r\n";
	_response += "Server: webserver\r\n";
	ss_content << content_length;
	_response += "Content-Length: " + ss_content.str() + "\r\n";
	if (extention == "php")
		_response += "Content-Type: " + _type[extention] + "\r\n";
	else
		_response += "Content-Type: " + _type[extention] + '/' + extention + "\r\n";
	_response += "Connection: close\r\n\r\n";
}

void Response::_fill_response(std::string const& path, size_t status_code, std::string const& message)
{
	std::string 	line;
	std::string		*tmp_resp = new std::string();

	if (status_code == 200)
	{
		_file.open(path);
		while (!_file.eof())
		{
			std::getline(_file, line);
			if (!_file.eof())
				line += "\r\n";
			*tmp_resp += line;
		}
		*tmp_resp += "\r\n";
	}
	else
	{
		std::stringstream	ss;
		std::string			buff;
		ss << status_code;
		ss >> buff;
		delete tmp_resp;
		tmp_resp = error_page(buff  + ' ' + message);
	}
	// set all the needed response header
	_set_headers(status_code, message, tmp_resp->length(), path);
	_response += *tmp_resp;
	delete tmp_resp;
}

bool Response::_file_is_good(bool fill_resp)
{
	if (_file_path.empty())
		_file_path = _root + '/' + _uri;
	if (open(_file_path.c_str(), O_RDONLY) < 0)
	{
		if (errno == 2 && fill_resp)
			_fill_response(".html", 404, "Not Found");
		else if (fill_resp)
			_fill_response(".html", 403, "Forbidden");
		return false;
	}
	return true;
}
std::string const& 	Response::get_response(void) const	{ return _response; }
