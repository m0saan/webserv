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

std::ostream &operator<<(std::ostream &os, std::vector<Server*> const &vec) {
    os << "[" << std::endl;
    for (int i = 0; i < vec.size(); ++i) {
        os << "\tport: " << vec[i]->_port << std::endl;
        os << "\thost: " << vec[i]->_host << std::endl;
        os << "\tserver_name: " << vec[i]->_server_name << std::endl;
        os << "\terror_page: " << vec[i]->_error_page << std::endl;
        os << "\tmax_file_size: " << vec[i]->_max_file_size << std::endl;
        os << "\ttime_out: " << vec[i]->_time_out << std::endl;
        os << "\t\tlocation: " << std::endl;
        os << " \t\tlocation__root: " << vec[i]->loc._root << std::endl;
        os << " \t\tlocation__allowed_method: " << vec[i]->loc._allowed_method << std::endl;
        os << " \t\tlocation__index: " << vec[i]->loc._index << std::endl;
        os << " \t\tlocation__auto_index: " << vec[i]->loc._auto_index << std::endl;
        os << " \t\tlocation__auth_basic: " << vec[i]->loc._auth_basic << std::endl;
    }
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
            std::make_pair("root", eLocationDirectives::LOC_ROOT),
            std::make_pair("allowed_method", eLocationDirectives::LOC_ALLOWED_METHODS),
            std::make_pair("index", eLocationDirectives::LOC_INDEX),
            std::make_pair("auto_index", eLocationDirectives::LOC_AUTO_INDEX),
            std::make_pair("auth_basic", eLocationDirectives::LOC_AUTH_BASIC),
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
    std::vector<Server*> globalConfig;
    std::ifstream ifs;
    const std::string filename("../webserv/config_files/webserv.config");
    ifs.open(filename, std::ios_base::in);

    std::string line;
    int i = -1, j = -1;
    if (ifs.is_open()) {
        while (std::getline(ifs, line)) {
            if (line.empty()) continue;
            std::vector<std::string> tokens = split(line);
            int directive = getDirective(tokens[0]);
            if (tokens[0] == "server")
                directive = -1;
            switch (directive) {
                case -1:
                    globalConfig.push_back(new Server());
                    ++i;
                    break;
                case eGlobalDirectives::PORT:
                    if (globalConfig[i]->_port != "null")
                        exitError("Erooor");
                    globalConfig[i]->_port = tokens[1];
                    break;
                case eGlobalDirectives::HOST:
                    if (globalConfig[i]->_host != "null")
                        exitError("Erooor");
                    globalConfig[i]->_host = tokens[1];
                    break;
                case eGlobalDirectives::SERVER_NAME:
                    if (globalConfig[i]->_server_name != "null")
                        exitError("Erooor");
                    globalConfig[i]->_server_name = tokens[1];
                    break;
                case eGlobalDirectives::ERROR_PAGE:
                    if (globalConfig[i]->_error_page != "null")
                        exitError("Erooor");
                    globalConfig[i]->_error_page = tokens[1];
                    break;
                case eGlobalDirectives::MAX_FILE_SIZE:
                    if (globalConfig[i]->_max_file_size != "null")
                        exitError("Erooor");
                    globalConfig[i]->_max_file_size = tokens[1];
                    break;
                case eGlobalDirectives::TIME_OUT:
                    if (globalConfig[i]->_time_out != "null")
                        exitError("Erooor");
                    globalConfig[i]->_time_out = tokens[1];
                    break;
                case eLocationDirectives::LOC_ROOT:
                    if (globalConfig[i]->_loc[j]->_root != "null")
                        exitError("Erooor");
                    globalConfig[i]->_loc[j]->_root = tokens[1];
                    break;
                case eLocationDirectives::LOC_ALLOWED_METHODS:
                    if (globalConfig[i]->_loc[j]->_allowed_method != "null")
                        exitError("Erooor");
                    globalConfig[i]->_loc[j]->_allowed_method = tokens[1];
                    break;
                case eLocationDirectives::LOC_INDEX:
                    if (globalConfig[i]->_loc[j]->_index != "null")
                        exitError("Erooor");
                    globalConfig[i]->_loc[j]->_index = tokens[1];
                    break;
                case eLocationDirectives::LOC_AUTO_INDEX:
                    if (globalConfig[i]->_loc[j]->_auto_index != "null")
                        exitError("Erooor");
                    globalConfig[i]->_loc[j]->_auto_index = tokens[1];
                    break;
                case eLocationDirectives::LOC_AUTH_BASIC:
                    if (globalConfig[i]->_loc[j]->_auth_basic != "null")
                        exitError("Erooor");
                    globalConfig[i]->_loc[j]->_auth_basic = tokens[1];
                    break;

                case eGlobalDirectives::LOCATION:
                    globalConfig[i]->_loc.push_back(new Location());
                    ++j;
                    break;
//                    if (tokens.size() == 3) globalConfig[tokens[0]] = tokens[1];
//                    else if (tokens.size() == 4) std::cout << "size is 3" << std::endl;
//                    else exitError("SYNTAX ERROR.");

//                case eGlobalDirectives::INVALID:
//                    exitError("SYNTAX ERROR.");
                default:
                    break;
                    // exitError("Invalid directive found in config file");
            }
//            std::cout << tokens << std::endl;
        }
    } else
        exitError("wrong config file path");
    std::cout << globalConfig << std::endl;
    return EXIT_SUCCESS;
}
