/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbani <mbani@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 13:41:20 by mbani             #+#    #+#             */
/*   Updated: 2021/12/15 16:41:47 by mbani            ###   ########.fr       */
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
		PORT = std::stoi(conf._port);
		if (conf._host.length() == 0)
			throw std::exception();
		else if (conf._host == "localhost")
			conf._host = "127.0.0.1";
	}
	catch (const std::exception &e)
	{
		return;
	}
	server_cli.push_back(new Sockets());
	(server_cli.back())->create_socket();
	(server_cli.back())->set_addr(PORT, conf._host);
	(server_cli.back())->bind_socket();
	(server_cli.back())->listen_socket();
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
	int fd = server_cli.back()->get_fd(); // get Fd
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
			return true;
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
			// std::cout << "request : \n" << (req_res.getMap())[fd].get_req().str() << std::endl;  

			(req_res.getMap())[fd].parseRequest(); // Parse Request
			std::map<std::string, std::vector<std::string> > _request_map = req_res.getMap()[fd].getMap();
			std::string host = (_request_map["Host"][0]).substr(0, _request_map["Host"][0].find(":"));
			std::string port = (_request_map["Host"][0]).substr(_request_map["Host"][0].find(":") + 1);
			host = host == "localhost" ? "127.0.0.1" : host;
			ServerConfig chosen_config = Utility::getRightConfig(port, host, _request_map["Host"][0], _request_map["SL"][1], _config);

			// std::cerr << "-------------------------------------------------------------" << std::endl;
			// std::cerr << chosen_config << std::endl;
			// std::cerr << "-------------------------------------------------------------" << std::endl;
            // std::cout << chosen_config << std::endl;
            /* mosan is done right here!! */
			// ToDo: check if the request is bad!!!!!!
			Response res(chosen_config, _request_map, req_res.getMap()[fd].getQueriesScriptName(), request.getBodyFD());
			try
			{
				if (!chosen_config._redirect.first.empty())
					res.Redirection();
				else if (_request_map["SL"][0] == "GET")
					res.Get_request();
				else if (_request_map["SL"][0] == "POST")
					res.Post_request();
				else
					res.Delete_request();
			}
			catch(std::bad_alloc const& e)
			{
				(void)e;
				res.bad_allocation();
			}
			catch(std::exception const& e)
			{
				(void)e;
				res.internal_error();
			}
			res._size = res.get_response().length();
            req_res.add_response(fd, res);
			/* mamoussa done! */
			req_res.set_fd(fd, false, true); // add client fd to write set
			req_res.remove_fd(fd, 1, 1);
		}
	}
	return true;
}

void Server::sendResponse(int fd)
{
	// send response
	// req_res.send_all(fd, res);
	
	
	// std::cout << req_res.getResponse(fd) << std::endl; 
	int sent;
	// sleep(5);
	sent = send(fd, (void *)(req_res.getResponse(fd).c_str() + req_res.get_res_bytes_sent(fd)), (req_res.get_response_length(fd) - req_res.get_res_bytes_sent(fd)), 0);	// std::cout << sent  << " " << req_res.get_response_length(fd) << std::endl;
		//ToDo: Handle Error case
	if (sent == -1) // send failed
	{
		perror("send ");
	}
	req_res.update_sent_bytes(fd, sent);
	// std::cout << "Sent : " << sent << std::endl;
	if (req_res.get_response_length(fd) == req_res.get_res_bytes_sent(fd)) // response is completely sent
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
		if (!req_res.select_fd())
			continue;
		for (size_t i = 0; i < server_cli.size(); ++i)
		{
			if (req_res.is_ready((server_cli[i])->get_fd(), 1)) // check if fd is ready to read
				if (!readFromFd((server_cli[i])->get_fd())) // return false in case of connection closed
					continue;
			if (req_res.is_ready((server_cli[i])->get_fd(), 0)) // check if fd is ready to write
				sendResponse((server_cli[i])->get_fd());
		}
	}
}

void Server::socketFree(int fd)
{
	std::vector<Sockets *>::iterator first(server_cli.begin());
	std::vector<Sockets *>::iterator last(server_cli.end());

	for (; first != last; ++first)
	{
		if ((*first)->get_fd() == fd)
		{
			delete *first;
			*first = NULL;
			server_cli.erase(first);
			// std::cout << "Freed " << std::endl;
		}
	}
	return;
}

Server::~Server()
{
}
