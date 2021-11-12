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
    std::map<const std::string, std::vector<std::string> > _requestMap;
public:

    Request();
    ~Request();
    Request(Request const& x);
    Request& operator=(Request const &x);

    std::map<const std::string, std::vector<std::string> > const& getMap() const;

    std::vector<std::string> const &getValue(std::string const & key);

    void parseRequest(std::stringstream & requestStream);

};

#endif //WEBSERV_REQUEST_HPP
