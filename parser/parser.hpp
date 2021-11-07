//
// Created by Mohammad  Boustta on 11/2/21.
//

#ifndef __PARSER_HPP__
#define __PARSER_HPP__

class eGlobalDirectives {
public:
        static const int PORT = 0;
        static const int HOST = 1;
        static const int SERVER_NAME = 2;
        static const int ERROR_PAGE = 3;
        static const int MAX_FILE_SIZE = 4;
        static const int TIME_OUT = 5;
        static const int LOCATION = 6;
        static const int INVALID = 7;
};

class eLocationDirectives : public eGlobalDirectives
{
public:
    static const int  LOC_ROOT = 9;
    static const int  LOC_ALLOWED_METHODS = 10;
    static const int  LOC_INDEX = 11;
    static const int  LOC_AUTO_INDEX = 12;
    static const int  LOC_AUTH_BASIC = 13;
};

struct Location {
    Location() : _root("null"), _allowed_method("null"), _index("null"), _auto_index("null"), _auth_basic("null") {}
    std::string _root, _allowed_method, _index, _auto_index, _auth_basic;
};

struct Server {
    Server() : _port("null"), _host("null"), _server_name("null"), _error_page("null"),_max_file_size("null"), _time_out("null") {}
    std::string _port, _host, _server_name, _error_page, _max_file_size, _time_out;
    Location loc;
    std::vector<Location*> _loc;
};

#endif // __PARSER_HPP__
