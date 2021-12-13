/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbani <mbani@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 10:31:11 by mbani             #+#    #+#             */
/*   Updated: 2021/12/13 11:16:21 by mbani            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "includes/request.hpp"
#include "includes/server.hpp"
#include "parser/parser.hpp"

int main(int ac, char **av) {
    if (ac != 2)
        exit(1);
    Request request;
    std::ifstream ifs;

    // std::cout << performParsing().size() << std::endl;
	std::vector<ServerConfig> vect = performParsing(av[1]);
    Server serv(vect);
    // ifs.open("./request", std::ios_base::in);
    // if (ifs.is_open()) {
    //     std::stringstream ss;
    //     std::string line;
    //     while (std::getline(ifs, line))
    //         ss << line << std::endl;

    //     request.parseRequest(ss);
    //     auto it = request.getMap().begin();

    //     for(; it != request.getMap().end(); ++it) {
    //         std::cout << it->first << " ";
    //         for (int i = 0; i < it->second.size(); ++i)
    //             std::cout << it->second[i] << " ";
    //         std::cout << std::endl;
    //     }
    // }
    serv.listen();

    return EXIT_SUCCESS;
}