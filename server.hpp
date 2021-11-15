/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbani <mbani@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 13:39:53 by mbani             #+#    #+#             */
/*   Updated: 2021/11/09 10:38:17 by mbani            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "sockets.hpp"
#include "request_response.hpp"

class server
{
	private:
		std::vector<sockets *> server_cli;
	public:
		server(std::vector<int> );
		void	listen();
		int		is_server(int fd, bool *is_client) const;
		~server();
};

