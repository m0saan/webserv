/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbani <mbani@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 11:26:12 by mbani             #+#    #+#             */
/*   Updated: 2021/11/08 11:47:53 by mbani            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sockets.hpp"

sockets::sockets(): fd(-1), PORT(-1), addrlen(-1), _is_client(0)
{
}

sockets::sockets(int fd, struct sockaddr_in socket_add, socklen_t 
socklen, bool is_client):fd(fd), socket_add(socket_add), addrlen(socklen), _is_client(is_client)
{}

sockets::sockets(const sockets &obj)
{
	*this = obj;
}

sockets& sockets::operator=(const sockets &obj)
{
	this->fd = obj.fd;
	this->PORT = obj.PORT;
	this->addrlen = obj.addrlen;
	this->_is_client = obj._is_client;
	this->socket_add.sin_family = obj.socket_add.sin_family;
	this->socket_add.sin_port = obj.socket_add.sin_port;
	this->socket_add.sin_addr.s_addr = obj.socket_add.sin_addr.s_addr;
	return *this;
}

int sockets::get_fd() const
{
	return this->fd;
}

int sockets::get_PORT() const
{
	return this->PORT;
}

struct sockaddr_in sockets::get_sockaddr() const
{
	return this->socket_add;
}

socklen_t sockets::get_addrlen() const
{
	return this->addrlen;
}

void	sockets::create_socket()
{
	//domain should be set to AF_INET for (IPv4) OR AF_INET6 for (IPv6)
	//type should be set to SOCK_STREAM for (TCP/IP) OR SOCK_DGRAM for (UDP/IP)
	//protocol should be set to 0 to indicate default protocol should be used
	if ((this->fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Socket");
		exit (-1);
	}
	// Make the fd Non-Blocking
	if (fcntl(this->fd, F_SETFL, O_NONBLOCK) < 0)
	{
		perror("FCNTL ");
		exit (-1);
	}
	int new_socket;
		setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &new_socket, sizeof(new_socket));
}

void	sockets::set_addr(int PORT, std::string ip)
{
	bzero((char *)& this->socket_add, sizeof(this->socket_add)); // intialize the struct
	this->socket_add.sin_family = AF_INET; // internet Family
	this->socket_add.sin_port = htons(PORT); // convert from host to network byte order
	this->socket_add.sin_addr.s_addr = inet_addr(ip.c_str()); // convert numbers-and-dots notation into binary form (in network byte order) 
	this->addrlen = sizeof(this->socket_add);
}

void	sockets::bind_socket() const
{
	if ((bind(this->fd, (const sockaddr *)&this->socket_add, this->addrlen)) < 0)
	{
		perror("Bind");
		exit (-1);
	}
}

void	sockets::listen_socket(int nbr_of_ports) const
{
	if ((listen(this->fd, nbr_of_ports)) < 0)
	{
		perror("listen");
		exit(-1);
	}
}

sockets* 	sockets::accept_connection(int sock_fd)
{
	int client_fd;
	struct sockaddr_in new_sock_add;
	socklen_t new_socklen;

	if ((client_fd = accept(sock_fd, (sockaddr *)&new_sock_add, &new_socklen)) < 0) // accept new connection
		throw std::exception();
	if (fcntl(client_fd, F_SETFL, O_NONBLOCK) < 0) // make the fd Non-Blocking
		throw std::exception();
	return (new sockets(client_fd, new_sock_add, new_socklen, 1));
}

bool	sockets::is_client()const
{
	return this->_is_client;
}

sockets::~sockets()
{}
