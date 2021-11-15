//
// Created by Mohammad  Boustta on 11/2/21.
//

#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <set>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "../includes/request.hpp"
#include <array>
#include "../includes/utility.hpp"
#include "../includes/status_code.h"

class Directives {
public:
        static const int PORT = 0;
        static const int HOST = 1;
        static const int SERVER_NAME = 2;
        static const int ERROR_PAGE = 3;
        static const int MAX_FILE_SIZE = 4;
        static const int TIME_OUT = 5;
        static const int LOCATION = 6;
        static const int REDIRECT = 7;
        static const int CGI = 8;
        static const int  ROOT = 9;
        static const int  ALLOWED_METHODS = 10;
        static const int  INDEX = 11;
        static const int  AUTO_INDEX = 12;
        static const int  AUTH_BASIC = 13;
        static const int  UPLOAD = 14;
        static const int  LOCATION_END = 99;
        static const int  SERVER_END = 100;

};

typedef enum error {
    INVALID_DIRECTIVE = -2,
}            e_error;

struct ServerConfig {

    ServerConfig() :
    _port("null"), _host("null"), _server_name("null"),
    _error_page("null"),_max_file_size("null"),
    _time_out("null"), _cgi("null"), _root("null"),
    _auto_index("null"), _auth_basic("null"), _redirect(), _index(),
    _allowed_method(), _location(), upload_pass() {}

    std::string _port;
    std::string _host;
    std::string _server_name;
    std::string _error_page;
    std::string _max_file_size;
    std::string _time_out;
    std::string _cgi;
    std::string _root;
    std::string _auto_index;
    std::string _auth_basic;

    std::pair<HTTP::StatusCodes, std::string> _redirect;
    std::vector<std::string> _index;
    std::set<std::string> _allowed_method;

    std::vector<ServerConfig*> _location;

    bool upload_pass;


};

int getDirective(std::string const &token);
std::ostream &operator<<(std::ostream &os, std::vector<std::string> const &vec);
std::ostream &operator<<(std::ostream &os, std::vector<ServerConfig *> const &vec);
void exitError(std::string const &error);
int getDirective(std::string const &token);
std::vector<ServerConfig *> performParsing();

#endif // __PARSER_HPP__
