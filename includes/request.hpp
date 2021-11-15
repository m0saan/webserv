//
// Created by Mohammad  Boustta on 11/12/21.
//

#ifndef __REQUEST_HPP__
#define __REQUEST_HPP__

#include <map>
#include <string>
#include <vector>
#include <sstream>

class Request {
private:
    std::map<std::string, std::vector<std::string> > _requestMap;
public:

    Request();
    ~Request();
    Request(Request const& x);
    Request& operator=(Request const &x);

    std::map<std::string, std::vector<std::string> > const& getMap() const;

    std::vector<std::string> const &getValue(std::string const & key);

    void parseRequest(std::stringstream & requestStream);

private:

    void _getHeader(const std::string &line);
    bool _isBody(std::string const &line, bool const& is_body) const;

    void _getBody(std::string &line);

    bool _isBodyEnd(const std::string &line) const;

    bool _isBodyStart(const std::string &line, bool is_body) const;
};

#endif // __REQUEST_HPP__
