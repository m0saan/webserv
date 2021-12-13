/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.hpp                                  :+:      :+:    :+:   */
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

class Sockets
{
	private:
		int 				fd;
		int 				PORT;
		struct sockaddr_in 	socket_add;
		socklen_t 			addrlen;
		bool				_is_client;
	public:
		Sockets();
		friend class server;
		Sockets(const Sockets &);
		Sockets(int fd, struct sockaddr_in socket_add, socklen_t, bool is_client);
		Sockets&		operator=(const Sockets &);
		int 				get_fd()								const;
		int 				get_PORT()								const;
		struct sockaddr_in	get_sockaddr()							const;
		socklen_t 			get_addrlen()							const;
		void				bind_socket() 							const;
		void				listen_socket()					 		const;
		void 				create_socket();
		void				set_addr(int PORT, std::string ip);
		Sockets*			accept_connection(int sock_fd);
		bool				is_client()								const;
		~Sockets();
};