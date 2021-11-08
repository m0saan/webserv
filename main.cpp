#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "parser/parser.hpp"
#include <array>

std::ostream &operator<<(std::ostream &os, std::vector<std::string> const &vec) {
    os << "[ ";
    for (int i = 0; i < vec.size(); ++i)
        os << vec[i] << " ";
    os << ']' << std::endl;
    return os;
}

std::ostream &operator<<(std::ostream &os, std::vector<Server *> const &vec) {
    os << "[" << std::endl;
    for (int i = 0; i < vec.size(); ++i) {
        std::cout << "--------------------- Server " << i + 1 << " ---------------------" << std::endl;

        os << "port: " << vec[i]->_port << std::endl;
        os << "host: " << vec[i]->_host << std::endl;
        os << "server_name: " << vec[i]->_server_name << std::endl;
        os << "error_page: " << vec[i]->_error_page << std::endl;
        os << "max_file_size: " << vec[i]->_max_file_size << std::endl;
        os << "time_out: " << vec[i]->_time_out << std::endl;

        for (size_t j = 0; j < vec[i]->_location.size(); j++) {
            os << "\tlocation" << std::endl;
            os << "\t\troot: " << vec[i]->_location[j]->_root << std::endl;
            os << "\t\tallowed_method: ";
            for (std::set<std::string>::iterator ut = vec[i]->_location[j]->_allowed_method.begin();
                 ut != vec[i]->_location[j]->_allowed_method.end(); ++ut)
                os << *ut << " ";
            os << std::endl;
            os << "\t\tindex: " << vec[i]->_location[j]->_index << std::endl;
            os << "\t\tauto_index: " << vec[i]->_location[j]->_auto_index << std::endl;
            os << "\t\tauth_basic: " << vec[i]->_location[j]->_auth_basic << std::endl;
        }

        std::cout << std::endl;
    }
    os << ']' << std::endl;
    return os;
}

void exitError(std::string const &error) {
    std::cout << error << std::endl;
    exit(EXIT_FAILURE);
}

int getDirective(std::string const &token) {
    static const std::array<std::pair<std::string, int>, 16> globalDirectives = {
            std::make_pair("port", Directives::PORT),
            std::make_pair("host", Directives::HOST),
            std::make_pair("server_name", Directives::SERVER_NAME),
            std::make_pair("error_page", Directives::ERROR_PAGE),
            std::make_pair("max_file_size", Directives::MAX_FILE_SIZE),
            std::make_pair("time_out", Directives::TIME_OUT),
            std::make_pair("location", Directives::LOCATION),
            std::make_pair("root", Directives::ROOT),
            std::make_pair("allowed_method", Directives::ALLOWED_METHODS),
            std::make_pair("index", Directives::INDEX),
            std::make_pair("cgi", Directives::CGI),
            std::make_pair("redirect", Directives::REDIRECT),
            std::make_pair("upload_pass", Directives::UPLOAD),
            std::make_pair("auto_index", Directives::AUTO_INDEX),
            std::make_pair("auth_basic", Directives::AUTH_BASIC),
            std::make_pair("]", Directives::LOCATION_END),
    };

    for (int i = 0; i < globalDirectives.size(); ++i)
        if (token == globalDirectives[i].first)
            return globalDirectives[i].second;
    return e_error::INVALID_DIRECTIVE;
}

std::vector<std::string> split(std::string const &line, char del = ' ') {
    std::vector<std::string> out;
    std::stringstream ss(line);
    std::string tmp;
    while (std::getline(ss, tmp, del)) {
        if (tmp == " " || tmp.empty())
            continue;
        out.push_back(tmp);
    }
    return out;
}

template<typename T>
void fillGlobalDirectives(T &field, T const &value) {
    if (field != "null")
        exitError("Error");
    field = value;
}

void parseLocation(int directive, std::vector<std::string> const &tokens, std::vector<Server *> &location) {

}


int main() {
    std::vector<Server *> globalConfig;
    std::ifstream ifs;
    const std::string filename("../webserv/config_files/webserv.config");
    ifs.open(filename, std::ios_base::in);

    std::string line;
    int i = -1, j = -1;
    bool isLocation = false;
    if (ifs.is_open()) {
        while (std::getline(ifs, line)) {
            if (line.empty())
                continue;

            std::vector<std::string> tokens = split(line);
            int directive = getDirective(tokens[0]);
            if (tokens[0] == "server")
                directive = -1;
            if (isLocation)
                parseLocation(directive, tokens, globalConfig[i]->_location);

            switch (directive) {
                case -1:
                    globalConfig.push_back(new Server());
                    ++i;
                    j = -1;
                    break;
                case Directives::PORT:
                    if (!isLocation)
                        fillGlobalDirectives(globalConfig[i]->_port, tokens[1]);
                    else
                        fillGlobalDirectives(globalConfig[i]->_location[j]->_port, tokens[1]);
                    break;
                case Directives::HOST:
                    if (!isLocation)
                        fillGlobalDirectives(globalConfig[i]->_host, tokens[1]);
                    else
                        fillGlobalDirectives(globalConfig[i]->_location[j]->_host, tokens[1]);
                    break;
                case Directives::SERVER_NAME:
                    if (!isLocation)
                        fillGlobalDirectives(globalConfig[i]->_server_name, tokens[1]);
                    else
                        fillGlobalDirectives(globalConfig[i]->_location[j]->_server_name, tokens[1]);
                    break;
                case Directives::ERROR_PAGE:
                    if (!isLocation)
                        fillGlobalDirectives(globalConfig[i]->_error_page, tokens[1]);
                    else
                        fillGlobalDirectives(globalConfig[i]->_location[j]->_error_page, tokens[1]);
                    break;
                case Directives::MAX_FILE_SIZE:
                    if (!isLocation)
                        fillGlobalDirectives(globalConfig[i]->_max_file_size, tokens[1]);
                    else
                        fillGlobalDirectives(globalConfig[i]->_location[j]->_max_file_size, tokens[1]);
                    break;
                case Directives::TIME_OUT:
                    if (!isLocation)
                        fillGlobalDirectives(globalConfig[i]->_time_out, tokens[1]);
                    else
                        fillGlobalDirectives(globalConfig[i]->_location[j]->_time_out, tokens[1]);
                    break;
                case Directives::ROOT:
                    if (!isLocation)
                        fillGlobalDirectives(globalConfig[i]->_root, tokens[1]);
                    else
                        fillGlobalDirectives(globalConfig[i]->_location[j]->_root, tokens[1]);
                    break;
                case Directives::ALLOWED_METHODS:
                    if (!globalConfig[i]->_location[j]->_allowed_method.empty())
                        exitError("Erooor");
                    if (!isLocation)
                        for (int k = 1; k < tokens.size(); ++k) globalConfig[i]->_allowed_method.insert(tokens[k]);
                    else
                        for (int k = 1; k < tokens.size(); ++k)
                            globalConfig[i]->_location[j]->_allowed_method.insert(tokens[k]);
                    break;
                case Directives::INDEX:
                    if (!isLocation)
                        std::copy(tokens.begin() + 1, tokens.end(), globalConfig[i]->_index.begin());
                    else std::copy(tokens.begin() + 1, tokens.end(), globalConfig[i]->_location[j]->_index.begin());
                    break;
                case Directives::AUTO_INDEX:
                    if (!isLocation)
                        fillGlobalDirectives(globalConfig[i]->_auto_index, tokens[1]);
                    else
                        fillGlobalDirectives(globalConfig[i]->_location[j]->_auto_index, tokens[1]);
                    break;
                case Directives::AUTH_BASIC:
                    if (!isLocation)
                        fillGlobalDirectives(globalConfig[i]->_auth_basic, tokens[1]);
                    else
                        fillGlobalDirectives(globalConfig[i]->_location[j]->_auth_basic, tokens[1]);
                    break;

                case Directives::LOCATION:
                    globalConfig[i]->_location.push_back(new Server());
                    ++j;
                    isLocation = true;
                    break;
                    //                    if (tokens.size() == 3) globalConfig[tokens[0]] = tokens[1];
                    //                    else if (tokens.size() == 4) std::cout << "size is 3" << std::endl;
                    //                    else exitError("SYNTAX ERROR.");

                    //                case Directives::INVALID:
                    //                    exitError("SYNTAX ERROR.");
                case Directives::LOCATION_END:
                    isLocation = false;
                    break;
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



