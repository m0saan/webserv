/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbani <mbani@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 11:17:05 by mbani             #+#    #+#             */
/*   Updated: 2021/11/08 11:48:47 by mbani            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <arpa/inet.h>

class sockets
{
	private:
		int 				fd;
		int 				PORT;
		struct sockaddr_in 	socket_add;
		socklen_t 			addrlen;
		bool				_is_client;
	public:
		sockets();
		friend class server;
		sockets(const sockets &);
		sockets(int fd, struct sockaddr_in socket_add, socklen_t, bool is_client);
		sockets&		operator=(const sockets &);
		int 				get_fd()								const;
		int 				get_PORT()								const;
		struct sockaddr_in	get_sockaddr()							const;
		socklen_t 			get_addrlen()							const;
		void				bind_socket() 							const;
		void				listen_socket(int nbr_of_ports) 		const;
		void 				create_socket();
		void				set_addr(int PORT, std::string ip);
		sockets*		accept_connection(int sock_fd);
		bool				is_client()								const;
		~sockets();
};