#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>


std::vector<char const*>	cgi_meta_var(void)
{
	std::vector<char const*> meta_var;
	std::string				str;
	/*
	 * SRC = Request (we will get this info from the request headers)
	 * The server MUST set this meta-variable if and only if the request is accompanied by a message body entity.
	 *	The CONTENT_LENGTH value must reflect the length of the message-body
	 */
	str = std::string("CONTENT_LENGHT") + '\n';
	meta_var.push_back(str.c_str());
	// _cgi_meta_var += "CONTENT_LENGHT=" + '\n';
	/*
	 * SRC = Request (we will get this info from the request headers)
	 * The server MUST set this meta-variable if an HTTP Content-Type field is present in the client request header.
	 */
	str = std::string("CONTENT_TYPE=") + '\n';
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
	str = std::string("PATH_INFO=") + '\n';
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
	str = std::string("QUERY_STRING=") + '\n';
	meta_var.push_back(str.c_str());
	/*
	 * SRC = Request
	 * Contains the method (as specified with the METHOD attribute in an HTML form) that is
	 * used to send the request. Example: GET
	 */
	str = std::string("REQUEST_METHOD=GET") + '\n';
	meta_var.push_back(str.c_str());
	/*
	 * SRC = Request
	 * The path part of the URL that points to the script being executed.
	 * It should include the leading slash. Example: /cgi-bin/hello.pgm
	 */
	str = std::string("SCRIPT_NAME=") + '\n';
	meta_var.push_back(str.c_str());
	/* 
	 * SRC = Conf
	 * Contains the server host name or IP address of the server. Example: 10.9.8.7
	 */
	str = std::string("SERVER_NAME=") + '\n';
	meta_var.push_back(str.c_str());
	/*
	 * Contains the port number to which the client request was sent.
	 */
	str = std::string("SERVER_PORT=") + '\n';
	meta_var.push_back(str.c_str());
	str = "SERVER_PROTOCOL=HTTP/1.1\n";
	meta_var.push_back(str.c_str());
	str = "SERVER_SOFTWARE=Webserv\n";
	meta_var.push_back(str.c_str());
	meta_var.push_back(NULL);
	return meta_var;
}

int main(void)
{
	int fd = open("index.php", O_RDONLY);
	pid_t pid;
	int pfd[2];

	pipe(pfd);
	if(!(pid = fork()))
	{
		std::vector<char const*> meta_var = cgi_meta_var();
		std::vector<char const*> args;
		std::string path;

		args.push_back("/Users/mamoussa/Desktop/brew/bin/php-cgi");
		args.push_back(NULL);
		path = "/Users/mamoussa/Desktop/brew/bin/php-cgi";
		close(pfd[0]);
		dup2(fd, 0);
		dup2(pfd[1], 1);
		if (execve(path.c_str(), const_cast<char *const*>(&(*args.begin()))
		, const_cast<char *const*>(&(*meta_var.begin()))) < 0)
		{
			meta_var.~vector();
			args.~vector();
			std::cout << strerror(errno) << std::endl;
			exit(1);
		}
	}
	wait(&pid);
	close(pfd[1]);
	std::string ans;
	char		buff[1024];
	int 		ret = 1;

	while ((ret = read(pfd[0], buff, 1024)))
		ans += buff;
	std::cout << ans << std::endl;
	return 0;
}