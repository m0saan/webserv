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

typedef enum transfer_type {
    CHUNKED,
    COMPLETED,
}           transfer_type;

class Request {

private:

    std::map<std::string, std::vector<std::string> > _RequestMap;
    std::stringstream		_req;
    long long 		_size;
    long long 		_content_length;
    long long		_header_length;
    long long		_max_body_size;
    transfer_type	_transfer_encoding;
    std::pair<std::string, std::string> _url_queries_scriptName;
    std::fstream            _body_stream;
    int                     _fd;
public:

    bool            _is_alive_connection;
    Request(long long max_size = 100000);
    ~Request();
    Request(Request const& x);
    // Request& operator=(Request const &x);

    std::map<std::string, std::vector<std::string> > const& getMap() const;

    std::pair<std::string, std::string>& getQueriesScriptName();

    std::vector<std::string> const &getValue(std::string const & key);

    const int &getBodyFD() const;

    void parseRequest();

    bool is_completed() const;

    void append(char *content, long long size);

    std::stringstream const & get_req();

    void getReqInfo(const std::string& );

    long long getContentLength(const std::string &str);

    size_t getHeaderLength(const std::string &str);

    void    resetRequest();

private:

    void _getHeader(const std::string &, std::string&, std::string&);

    bool _isBody(std::string const &line, bool const& is_body) const;

    void _getBody(std::string &line, bool is_chunked);

    bool _isBodyEnd(const std::string &line) const;

    bool _isBodyStart(const std::string &line, bool is_body) const;

    bool _isChunckStart(const std::string& line) const;

};

#endif // __Request_HPP__
