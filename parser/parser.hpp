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

#endif // __PARSER_HPP__
