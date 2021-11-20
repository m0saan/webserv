/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbani <mbani@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 13:41:20 by mbani             #+#    #+#             */
/*   Updated: 2021/11/20 15:56:19 by mbani            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <string>
// void server::locationConfig(ServerConfig *conf)
// {

// }

void Server::initConfig(ServerConfig* conf, size_t size)
{
	int PORT;
	if (!conf || conf->_port == "null" || conf->_host == "null" || size == 0)
		return ;
	try
	{
		/* code */
		PORT = std::stoi(conf->_port);
		if (conf->_host.length() == 0)
			throw std::exception();
		else if (conf->_host == "localhost")
			conf->_host = "127.0.0.1";
	}
	catch(const std::exception& e)
	{
		return ;
	}
	server_cli.push_back(new sockets());
	(server_cli.back())->create_socket();
	(server_cli.back())->set_addr(PORT, conf->_host);
	(server_cli.back())->bind_socket();
	(server_cli.back())->listen_socket(size);
	return ;
}

Server::Server(std::vector<ServerConfig*> config)
{	
	for(size_t i = 0 ; i < config.size(); ++i)
	{
		initConfig(config[i], config.size());
	}
	
}

int Server::is_server(int fd, bool *is_client) const
{
	// returns index of fd and check if it's client or server
	for(size_t i = 0; i < server_cli.size(); ++i)
	{
		if (fd == server_cli[i]->get_fd())
		{
			*is_client = server_cli[i]->is_client();
			return i;
		}
	}
	return -1;
}

void	Server::listen()
{

	if (this->server_cli.empty())
	{
		std::cout << "Error config " << std::endl;
		exit(1);
	}
	request_response req_res;
	int position;
	bool is_client;
	std::string req;
	Request request;
	for(size_t i = 0; i < server_cli.size(); ++i)
		req_res.set_fd((server_cli[i])->get_fd(), true, false);
	while(1)
	{
		req_res.update_set();
		// std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n";
		req_res.select_fd();
		for(int i = 0; i <= req_res.get_maxfd(); ++i)
		{
			if (req_res.is_ready(i, 1)) // check if fd is ready to read 
			{
				position = is_server(i, &is_client);
				if (!is_client)
				{
					// server socket
					try
					{
						server_cli.push_back(server_cli[position]->accept_connection(i)); // accept connections && add client to server_cli obj
						req_res.set_fd((server_cli.back())->get_fd(), true, true); // add client fd to read set && create req_fd
					}
					catch(const std::exception& e)
					{
						std::cerr << "Cannot Accept New Connection" << std::endl;;
					}
				}
				else
				{
					//	client socket
					std::cout << "still checking ... " << i  << std::endl;
						if (!req_res.receive(i, *this)) // if connection is closed or invalid socket
							continue; 
					// parse request
					if (req_res.req_completed(i))
					{
						(req_res.getMap())[i].parseRequest();
						auto it = req_res.getMap()[i].getMap().begin();

					    for(; it != req_res.getMap()[i].getMap().end(); ++it) {
					        std::cout << it->first << " ";
					        for (int i = 0; i < it->second.size(); ++i)
					            std::cout << it->second[i] << " ";
					        std::cout << std::endl;
						}
						// std::cout << 
						req_res.set_fd(i, false, true); // add client fd to write set
						// if (close)
						// {
							req_res.remove_fd(i, 1, 1); // clear if from read set
						// }
					}
				}
			}
			if (req_res.is_ready(i, 0)) // check if fd is ready to write
			{
				// send response
				std::string res = "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\nContent-Type: text/plain\r\n";
				req_res.send_all(i, res);
				res = "\r\n5\r\nHello\r\n8\r\nGood bye\r\n0\r\n\r\n";
				req_res.send_all(i, res);
				// if (close)
				// {
				req_res.remove_fd(i, 0, 0);
				req_res.close_connection(i);
				// socketFree(i);
				// }
				// else
				// {
				// req_res.reset(i);
				// req_res.remove_fd(i, 0, 0);
				// req_res.set_fd(i, 1, 1);
			}
		}
	}
}


void Server::socketFree(int fd)
{
	std::vector<sockets *>::iterator first(server_cli.begin());
	std::vector<sockets *>::iterator last(server_cli.end());

	for(; first != last; ++first)
	{
		if ((*first)->get_fd() == fd)
		{
			delete *first;
			*first = NULL;
			server_cli.erase(first);
			std::cout << "Freed " << std::endl;
		}
	}
	return ;
}

Server::~Server()
{}
