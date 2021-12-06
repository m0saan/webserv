//
// Created by Mohammad  Boustta on 11/12/21.
//

#ifndef __UTILITY_HPP__
#define __UTILITY_HPP__

#include <string>
#include <map>
#include <vector>
#include "../parser/parser.hpp"
#include <algorithm>

class ServerConfig;

class Utility {
public:
    static std::vector<std::string> split(std::string const &line, char del = ' ') {
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

    static std::pair<bool, int> hasQueries(std::string const &url) {
        size_t res = url.find('?');
        return (std::make_pair(res != std::string::npos, res));
    }

    static std::string getQueries(std::string const &url, std::size_t querie_start) {
        return url.substr(querie_start + 1);
    }

    static std::string urlDecode(std::string &SRC) {
        std::string ret;
        char ch;
        int i, ii;
        for (i = 0; i < SRC.length(); i++) {
            if (int(SRC[i]) == 37) {
                sscanf(SRC.substr(i + 1, 2).c_str(), "%x", &ii);
                ch = static_cast<char>(ii);
                ret += ch;
                i = i + 2;
            } else
                ret += SRC[i];
        }
        return (ret);
    }

    static std::string getScriptName(std::string &url) {
        std::vector<std::string> res = Utility::split(url, '/');
        return res[res.size() - 1];
    }


    static bool startWith(std::string const &haystack, std::string const &needle) {
        for (size_t i = 0; i < needle.length(); i++)
            if (needle.at(i) != haystack.at(i))
                return false;
        return true;

    }
};

#endif // __UTILITY_HPP__
