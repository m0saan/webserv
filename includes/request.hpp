//
// Created by Mohammad  Boustta on 11/12/21.
//

#ifndef __Request_HPP__
#define __Request_HPP__

#include <map>
#include <string>
#include <vector>
#include <sstream>


class Request {
private:
    typedef enum transfer_type
    {
        CHUNKED, COMPLETED
    } transfer_type;

    std::map<std::string, std::vector<std::string> > _RequestMap;
    std::string		_req;
    long long 		_size;
    long long 		_content_length;
    long long		_header_length;
    long long		_max_body_size;
    transfer_type	transfer_encoding;
public:

    Request();
    Request(long long max_size = 100000);
    ~Request();
    Request(Request const& x);
    Request& operator=(Request const &x);

    std::map<std::string, std::vector<std::string> > const& getMap() const;

    std::vector<std::string> const &getValue(std::string const & key);

    void parseRequest(std::stringstream & RequestStream);

    bool is_completed() const;

    void append(char *content, long long size);

    std::string get_req()const;

    void getReqInfo(std::string );

    long long getContentLength(std::string str);

    size_t getHeaderLength(std::string str);

private:

    void _getHeader(const std::string &line);
    bool _isBody(std::string const &line, bool const& is_body) const;

    void _getBody(std::string &line);

    bool _isBodyEnd(const std::string &line) const;

    bool _isBodyStart(const std::string &line, bool is_body) const;
};

#endif // __Request_HPP__
