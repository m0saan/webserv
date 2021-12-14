/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbani <mbani@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 13:39:53 by mbani             #+#    #+#             */
/*   Updated: 2021/12/14 14:03:55 by mbani            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "sockets.hpp"
#include "../includes/parser.hpp"
#include "requestresponse.hpp"

class Server
{
	private:
		std::vector<Sockets *> server_cli;
		void initConfig(ServerConfig& conf, size_t size);
		RequestResponse req_res;
		std::vector<ServerConfig> &_config;
	public:
		Server(std::vector<ServerConfig>& );
		void	listen();
		int		is_server(int fd, bool *is_client) const;
		void	socketFree(int fd);
		bool  	readFromFd(int fd);
		void	sendResponse(int fd);
		void 	emergencyFree();
		~Server();
};

