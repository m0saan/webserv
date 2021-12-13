/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbani <mbani@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 10:31:11 by mbani             #+#    #+#             */
/*   Updated: 2021/12/13 13:51:30 by mbani            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "includes/request.hpp"
#include "includes/server.hpp"
#include "parser/parser.hpp"

int main(int ac, char **av) {
    if (ac != 2)
        exit(1);

	std::vector<ServerConfig> vect = performParsing(av[1]);
    Server serv(vect);
    serv.listen();

    return EXIT_SUCCESS;
}