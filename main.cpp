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


void testRequest() {
    Request request;
    std::ifstream ifs;
    ifs.open("./request", std::ios_base::in);
    if (ifs.is_open()) {
        std::stringstream ss;
        std::string line;
        while (std::getline(ifs, line))
            ss << line << std::endl;

        request.parseRequest(ss);
        auto it = request.getMap().begin();

        for(; it != request.getMap().end(); ++it) {
            std::cout << it->first << " ";
            for (int i = 0; i < it->second.size(); ++i)
                std::cout << it->second[i] << " ";
            std::cout << std::endl;
        }
    }
}


int main() {
    auto res = performParsing();
     std::cout << res << std::endl;
    return EXIT_SUCCESS;
}



