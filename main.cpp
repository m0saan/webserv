#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "parser/parser.hpp"
#include "includes/request.hpp"
#include "includes/utility.hpp"
#include "includes/server.hpp"


int main(int ac, char **av) {
    if (ac != 2)
        exit(1);
    Request request;
    std::ifstream ifs;

    // std::cout << performParsing().size() << std::endl;
    Server serv(performParsing(av[1]));
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


