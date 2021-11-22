/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_response.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbani <mbani@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 08:37:44 by mbani             #+#    #+#             */
/*   Updated: 2021/11/21 12:08:26 by mbani            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#define BUFFER_SIZE 1024 * 100
#include "request.hpp"
#include <map>

class Server;

class request_response
{
	private:
		fd_set	read_fd, tmp_read, write_fd, tmp_write;
		int		max_fd;
		std::map<int , Request> req_fd;
	public:
		request_response();
		void			update_set();
		void			set_fd(int fd, bool to_read, bool is_client);
		void			remove_fd(int fd, bool to_read, bool is_client, bool _close = false);
		bool			select_fd();
		int				get_maxfd() 					const;
		int				is_ready(int fd, bool to_read);
		bool 			receive(int fd, Server &server);
		void			send_all(int fd, std::string res);
		void			close_connection(int fd)		 const;
		bool			req_completed(int fd);
		const std::stringstream&		get_req(int fd);
		void			reset(int fd);
		std::map<int , Request>& getMap();
		~request_response();
};


#include "server.hpp"