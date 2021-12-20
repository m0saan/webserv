//
// Created by Mohammad  Boustta on 11/12/21.
//

#ifndef __Request_HPP__
#define __Request_HPP__

#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

typedef enum transfer_type
{
    CHUNKED,
    COMPLETED,
} transfer_type;

typedef enum method
{
    GET,
    DELETE,
    POST,
} method;

class Request
{

private:
    std::map<std::string, std::vector<std::string> > _RequestMap;
    long long _size;
    long long _content_length;
    long long _header_length;
    bool _content_type;
    long long _max_body_size;
    transfer_type _transfer_encoding;
    std::pair<std::string, std::string> _url_queries_scriptName;
    int _fd;
    bool _bad_request_found;
    bool _is_chunked_completed;
    bool _is_forbiden_method;
    std::fstream _req_file;
    std::vector<std::string> _allowed_http_methods;
    method _req_method;
    std::string _req_header;

public:
    std::string _req_filename;
    bool _is_alive_connection;
    Request(long long max_size = 100000);
    ~Request();
    Request(Request const &x);
    // Request& operator=(Request const &x);

    std::map<std::string, std::vector<std::string> > const &getMap() const;

    std::pair<std::string, std::string> &getQueriesScriptName();

    std::vector<std::string> const &getValue(std::string const &key);

    const int &getBodyFD() const;

    const bool &getIsFobiddenMethod() const;

    bool isBadRequest();

    void parseRequest();

    bool is_completed() const;

    void append(char *content, long long size, int fd);

    std::stringstream const &get_req();

    void getReqInfo();

    long long getContentLength();

    size_t getHeaderLength();

    void setContentType();
    void resetRequest();
    std::string generateFilename(int fd);

private:
    void _getHeader(const std::string &, std::string &, std::string &);

    bool _isBody(std::string const &line, bool const &is_body) const;

    void _getBody(std::string &line, bool is_chunked);

    bool _isChunckStart(const std::string &line) const;

    void _checkForBadRequest();

    bool is_allowed_method(std::string const &http_method);

    bool _canGetBoundary(std::vector<std::string> &tokens, std::string &http_method, std::string &boundary);

};

#endif // __Request_HPP__
