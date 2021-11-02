#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <stack>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include "parser/parser.hpp"
#include <array>

std::ostream &operator<<(std::ostream &os, std::vector<std::string> const &vec) {
    os << "[ ";
    for (int i = 0; i < vec.size(); ++i)
        os << vec[i] << " ";
    os << ']' << std::endl;
    return os;
}

void exitError(std::string const &error) {
    std::cout << error << std::endl;
    exit(EXIT_FAILURE);
}

int getDirective(std::string const &token) {
    static const std::array<std::pair<std::string, int>, 12> globalDirectives = {
            std::make_pair("port", eGlobalDirectives::PORT),
            std::make_pair("host", eGlobalDirectives::HOST),
            std::make_pair("server_name", eGlobalDirectives::SERVER_NAME),
            std::make_pair("error_page", eGlobalDirectives::ERROR_PAGE),
            std::make_pair("max_file_size", eGlobalDirectives::MAX_FILE_SIZE),
            std::make_pair("time_out", eGlobalDirectives::TIME_OUT),
            std::make_pair("location", eGlobalDirectives::LOCATION),
            std::make_pair("loc_root", eLocationDirectives::LOC_ROOT),
            std::make_pair("loc_allowed_methods", eLocationDirectives::LOC_ALLOWED_METHODS),
            std::make_pair("loc_index", eLocationDirectives::LOC_INDEX),
            std::make_pair("loc_auto_index", eLocationDirectives::LOC_AUTO_INDEX),
            std::make_pair("loc_auth_basic", eLocationDirectives::LOC_AUTH_BASIC),
};

    for (int i = 0; i < globalDirectives.size(); ++i)
        if (token == globalDirectives[i].first) return globalDirectives[i].second;
    return eGlobalDirectives::INVALID;
}

std::vector<std::string> split(std::string const &line, char del = ' ') {
    std::vector<std::string> out;
    std::stringstream ss(line);
    std::string tmp;
    while (std::getline(ss, tmp, del)) {
        if (tmp == " " || tmp.empty()) continue;
        out.push_back(tmp);
    }
    return out;
}

int main() {
    std::map<std::string, std::string> globalConfig;
    std::ifstream ifs;
    const std::string filename("../webserv/config_files/webserv.config");
    ifs.open(filename, std::ios_base::in);

    std::string line;
    if (ifs.is_open()) {
        while (std::getline(ifs, line)) {
            if (line.empty()) continue;
            std::vector<std::string> tokens = split(line);
            int directive = getDirective(tokens[0]);
            switch (directive) {

                    case eGlobalDirectives::PORT:
                    case eGlobalDirectives::HOST:
                    case eGlobalDirectives::SERVER_NAME:
                    case eGlobalDirectives::ERROR_PAGE:
                    case eGlobalDirectives::MAX_FILE_SIZE:
                    case eGlobalDirectives::TIME_OUT:
                        if (tokens.size() == 2) globalConfig[tokens[0]] = tokens[1];
                        else exitError("SYNTAX ERROR.");
                    break;

                case eGlobalDirectives::LOCATION:
                default:
                    break;
                // exitError("Invalid directive found in config file");
            }
//            std::cout << tokens << std::endl;
        }
    } else
        exitError("wrong config file path");

    std::map<std::string, std::string>::iterator  it = globalConfig.begin();
    for (; it != globalConfig.end(); ++it)
        std::cout << '(' << it->first << " " << it->second << ')' << std::endl;
    return EXIT_SUCCESS;
}
