#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>

#include "includes/parser.hpp"
#include "includes/request.hpp"
#include "includes/utility.hpp"
#include "includes/server.hpp"

int main(int ac, char **av) {
    if (ac != 2)
        exitError("message: wrong arguments."); 

    std::vector<ServerConfig> vect = performParsing(av[1]);
    Server serv(vect);
    serv.listen();
    return EXIT_SUCCESS;
}