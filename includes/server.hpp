/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbani <mbani@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 13:39:53 by mbani             #+#    #+#             */
/*   Updated: 2021/11/16 09:19:45 by mbani            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "sockets.hpp"
#include "request_response.hpp"
#include "parser.hpp"

class Server
{
	private:
		std::vector<sockets *> server_cli;
		void initConfig(ServerConfig* conf, size_t size);
		// void locationConfig();
	public:
		Server(std::vector<ServerConfig*> );
		void	listen();
		int		is_server(int fd, bool *is_client) const;
		~Server();
};

