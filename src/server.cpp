/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbani <mbani@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 13:41:20 by mbani             #+#    #+#             */
/*   Updated: 2021/12/19 20:26:49 by mbani            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"
#include "../includes/response.hpp"
#include "../includes/utility.hpp"


std::ostream& operator<<(std::ostream& os, std::vector<std::string>const &vec) {
	os << "[ ";
	for (size_t i = 0; i < vec.size(); i++)
		os << vec[i] << " ";
	os << "]";	
	return os;
}

std::ostream& operator<<(std::ostream& os, std::set<std::string>const &s) {
	os << "[ ";
	std::set<std::string>::iterator start = s.begin();
	for (; start != s.end(); ++start)
		os << *start << " ";
	os << "]";	
	return os;
}

std::ostream& operator<<(std::ostream& os, std::map<std::string, std::string> &m) {
	os << "[ ";
	std::map<std::string, std::string>::iterator start = m.begin();
	for (; start != m.end(); ++start)
		os << '(' << start->first << ", " << start->second  << ')' << " ";
	os << "]";	
	return os;
}

std::ostream &operator<<(std::ostream &os, ServerConfig const &conf) {
    std::cout << "port: " << conf._port  << std::endl;
    std::cout << "host: " << conf._host  << std::endl;
    std::cout << "server name: " << conf._server_name << std::endl;
	// std::cout << "error_page: " << conf._error_page << std::endl;
	std::cout << "auto index: " << conf._index << std::endl;
	std::cout << "allowed methods: " << conf._allowed_method << std::endl;
    std::cout << "cgi path: " << conf._cgi << std::endl;
    std::cout << "redirect: " << conf._redirect.first << ' ' << conf._redirect.second << std::endl;
    std::cout << "auto index: " << conf._auto_index  << std::endl;
	std::cout << "upload store: " << conf._upload_store << std::endl;
    std::cout << "location path: " << conf._loc_path  << std::endl;
    // std::cout << " location path" << conf._allowed_method  << std::endl;
    std::cout << " root: " << conf._root  << std::endl;
    return os;
}

void Server::initConfig(ServerConfig &conf, size_t size)
{
	int PORT;
	if (conf._port == "null" || conf._host == "null" || size == 0)
		return;
	try
	{
		/* code */
		std::stringstream tmp_stream(conf._port);
		tmp_stream >> PORT;
		if (conf._host.length() == 0)
			throw std::exception();
		else if (conf._host == "localhost" || conf._host == "127.0.0.1")
			conf._host = "127.0.0.1";
		else
			conf._host = "0.0.0.0";
	}
	catch (const std::exception &e)
	{
		return;
	}
	if (std::find(_opened_ports.begin(), _opened_ports.end(), PORT) != _opened_ports.end()) // Port already opened
		return;
	server_cli.push_back(new Sockets());
	(server_cli.back())->create_socket();
	(server_cli.back())->set_addr(PORT, conf._host);
	(server_cli.back())->bind_socket();
	(server_cli.back())->listen_socket();
	_opened_ports.push_back(PORT);
	return;
}

Server::Server(std::vector<ServerConfig> &config) : _config(config)
{
	for (size_t i = 0; i < config.size(); ++i)
	{
		initConfig(config[i], config.size());
	}
}

int Server::is_server(int fd, bool *is_client) const
{
	// returns index of fd and check if it's client or server
	for (size_t i = 0; i < server_cli.size(); ++i)
	{
		if (fd == server_cli[i]->get_fd())
		{
			*is_client = server_cli[i]->is_client();
			return i;
		}
	}
	return -1;
}

void Server::emergencyFree() // !CALL THE POLICE.
{
	int fd = server_cli.back()->get_fd();  // get Fd
	(req_res.getMap())[fd].resetRequest(); // clear request content
	// remove fd from read/write sets
	req_res.remove_fd(fd, true, true, true);
	req_res.remove_fd(fd, false, true, true);
	socketFree(fd);
	req_res.close_connection(fd);
}

bool Server::readFromFd(int fd)
{
	int position;
	bool is_client;
	position = is_server(fd, &is_client);
	if (!is_client) // Server Socket
		try
		{
			server_cli.push_back(server_cli[position]->accept_connection(fd)); // accept connections && add client to server_cli obj
			req_res.set_fd((server_cli.back())->get_fd(), true, true);		   // add client fd to read set && create req_fd
		}
		catch (std::bad_alloc &e)
		{
			emergencyFree();
			return false;
		}
		catch (const std::exception &e)
		{
			std::cerr << "Cannot Accept New Connection" << std::endl;
			return false;
		}
	else // Client Socket
	{
		if (!req_res.receive(fd, *this)) // if connection is closed or invalid socket
			return false;
		if (req_res.req_completed(fd))
		{
			// std::cout << "completed " ;
			(req_res.getMap())[fd].parseRequest(); // Parse Request
			std::map<std::string, std::vector<std::string> > _request_map = req_res.getMap()[fd].getMap();
			std::map<std::string, std::vector<std::string> >::iterator start = _request_map.begin();

			for (; start != _request_map.end(); start++)
				std::cout << '(' << start->first << ", " << start->second << ')' << std::endl;
			

			bool is_bad_request(false);
			ServerConfig chosen_config;
			bool is_body_size_exceeded(false);

			if (!((req_res.getMap())[fd].getIsFobiddenMethod()))
			{
				if (_request_map.count("Host") == 0)
					is_bad_request = true;
				if (!is_bad_request)
				{
					std::string host_key = _request_map["Host"][0];
					std::string host = host_key.substr(0, host_key.find(":")); // localhost
					std::string port = host_key.substr(host_key.find(":") + 1);
					host = host == "localhost" ? "127.0.0.1" : host;

					chosen_config = Utility::getRightConfig(port, host, host_key, _request_map["SL"][1], _config);

					std::stringstream tmp_convert;
					long request_content_length = 0;
					long config_content_length = 0;
					if (_request_map.count("Content-Length"))
					{
						tmp_convert << _request_map["Content-Length"][0];
						tmp_convert >> request_content_length;
						tmp_convert.clear();

						tmp_convert << chosen_config._max_file_size;
						tmp_convert >> config_content_length;
						config_content_length = chosen_config._max_file_size.empty() ? 1000000 : config_content_length;
					}
					if (request_content_length > config_content_length)
						is_body_size_exceeded = true;
				}
			}
			Response res(chosen_config, _request_map, req_res.getMap()[fd].getQueriesScriptName(), (req_res.getMap())[fd].getBodyFD(), (req_res.getMap())[fd].getIsFobiddenMethod());
			try
			{
				if (is_bad_request)
					res.handleBadRequest();
				else if (is_body_size_exceeded)
					res.handleMaxBodySize();
				else if ((req_res.getMap())[fd].getIsFobiddenMethod())
					res.Forbidden_method();
				else if (!chosen_config._redirect.first.empty())
					res.Redirection();
				else if (_request_map["SL"][0] == "GET")
					res.Get_request();
				else if (_request_map["SL"][0] == "POST")
					res.Post_request();
				else if (_request_map["SL"][0] == "DELETE")
					res.Delete_request();
			}

			catch (std::bad_alloc const &e)
			{
				(void)e;
				res.bad_allocation();
			}
			catch (std::exception const &e)
			{
				(void)e;
				std::cerr << e.what() << std::endl;
				res.internal_error();
			}

			// res._size = res.get_response().length();
			req_res.add_response(fd, res);

			req_res.set_fd(fd, false, true); // add client fd to write set
			req_res.remove_fd(fd, 1, 1);
		}
	}
	return true;
}

void Server::sendResponse(int fd)
{
	int sent;
	// sleep(5);
	std::cout << "Send from " << req_res.get_res_bytes_sent(fd) << " remaining bytes " << (req_res.get_response_length(fd) - req_res.get_res_bytes_sent(fd)) << std::endl;
	sent = send(fd, (void *)(req_res.getResponse(fd).c_str() + req_res.get_res_bytes_sent(fd)), (req_res.get_response_length(fd) - req_res.get_res_bytes_sent(fd)), 0); // std::cout << sent  << " " << req_res.get_response_length(fd) << std::endl;
	int ret = req_res.isResponseCompleted(fd);
	// std::cout << "ret " << ret << std::endl;
	if (ret != 0)
	{
		req_res.append_response(fd);
		// std::cout << "New Length " << req_res.get_response_length(fd) << std::endl;
	}																																			//ToDo: Handle Error case 0 and -1
	if (sent == -1)																																						// send failed
	{
		perror("send ");
		req_res.remove_fd(fd, 1, 1, 1); // erase client req object from map
		req_res.remove_fd(fd, 0, 1, 1); // remove client from write fd and erase res object from map
		req_res.close_connection(fd);
		socketFree(fd);
		return;
	}
	req_res.update_sent_bytes(fd, sent);
	if (req_res.get_response_length(fd) == (ssize_t)req_res.get_res_bytes_sent(fd) && ret == 0) // response is completely sent
	{
		if (!req_res.getMap()[fd]._is_alive_connection) // close connection
		{
			req_res.remove_fd(fd, 1, 1, 1); // erase req object from map
			req_res.remove_fd(fd, 0, 1, 1); // remove from write fd and erase res object from map
			req_res.close_connection(fd);
			socketFree(fd);
		}
		else // keep alive connection
		{
			req_res.reset(fd);
			req_res.remove_fd(fd, 0, 0);
			req_res.set_fd(fd, 1, 1);
		}
	}
}

void Server::listen()
{
	if (this->server_cli.empty()) // config isn't provided
	{
		std::cerr << "Error config " << std::endl;
		exit(1);
	}
	for (size_t i = 0; i < server_cli.size(); ++i) // Init FD_SET
		req_res.set_fd((server_cli[i])->get_fd(), true, false);
	while (1) // Server Loop
	{
		req_res.update_set();
		if (!req_res.select_fd()) // poll failure
			continue;
		for (size_t i = 0; i < server_cli.size(); ++i)
		{
			if (req_res.is_ready((server_cli[i])->get_fd(), 1)) // check if fd is ready to read
				if (!readFromFd((server_cli[i])->get_fd()))		// return false in case of connection closed
					continue;
			if (req_res.is_ready((server_cli[i])->get_fd(), 0)) // check if fd is ready to write
				sendResponse((server_cli[i])->get_fd());
		}
	}
}

void Server::socketFree(int fd)
{
	// remove sockket
	std::vector<Sockets *>::iterator first(server_cli.begin());

	for (; first != server_cli.end(); ++first)
	{
		if (*first && (*first)->get_fd() == fd)
		{
			delete *first;
			*first = NULL;
			server_cli.erase(first);
			return;
		}
	}
	return;
}

Server::~Server()
{
}
