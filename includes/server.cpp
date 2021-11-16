/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbani <mbani@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 13:41:20 by mbani             #+#    #+#             */
/*   Updated: 2021/11/14 17:52:27 by mbani            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

server::server(std::vector<int> PORT)
{
	for(size_t i = 0; i < PORT.size(); ++i)
	{
		server_cli.push_back(new sockets());
		server_cli[i]->create_socket();
		server_cli[i]->set_addr(PORT[i], "0.0.0.0");
		server_cli[i]->bind_socket();
		server_cli[i]->listen_socket(PORT.size());
	}
}

int server::is_server(int fd, bool *is_client) const
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

void	server::listen()
{
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


server::~server()
{}