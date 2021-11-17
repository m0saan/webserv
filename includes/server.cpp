/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbani <mbani@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 13:41:20 by mbani             #+#    #+#             */
/*   Updated: 2021/11/16 10:18:22 by mbani            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <string>
// void server::locationConfig(ServerConfig *conf)
// {

// }

void Server::initConfig(ServerConfig* conf, size_t size)
{
	if (!conf || conf->_port == "null" || conf->_host == "null" || size == 0)
		return ;
	int PORT;
	std::cout << conf->_port << std::endl;
	std::cout << conf->_host << std::endl;
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
	// std::cout << "------------------>Server setup PORT : " << PORT << " HOST : " << conf->_host << std::endl;
	server_cli.push_back(new sockets());
	(server_cli.back())->create_socket();
	(server_cli.back())->set_addr(PORT, conf->_host);
	(server_cli.back())->bind_socket();
	(server_cli.back())->listen_socket(size);
	// std::cout << "------------------>Server setup PORT : " << PORT << " HOST : " << conf->_host << std::endl;
	if (conf->_location.size() > 0)
	{
		std::cout << conf->_location.size() << std::endl;
		for(size_t i = 0; conf->_location.size(); ++i)
			initConfig(conf->_location[i], conf->_location.size());
	}
	return ;
}

Server::Server(std::vector<ServerConfig*> config)
{	
	for(size_t i = 0 ; i < config.size(); ++i)
	{
		std::cout << "_location : " << config[i]->_location.size() << std::endl;
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
	
	for(size_t i = 0; i < server_cli.size(); ++i)
		req_res.set_fd((server_cli[i])->get_fd(), true, false);
	while(1)
	{
		req_res.update_set();
		std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n";
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
					req_res.receive(i);
					// parse request
					if (req_res.req_completed(i))
					{
						std::cout << req_res.get_req(i) << std::endl;
						req_res.set_fd(i, false, true); // add client fd to write set
						req_res.remove_fd(i, 1, 1); // clear if from read set
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
				req_res.remove_fd(i, 0, 0);
				req_res.close_connection(i);
			}
		}
	}
}


Server::~Server()
{}