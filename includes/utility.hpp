//
// Created by Mohammad  Boustta on 11/12/21.
//

#ifndef __UTILITY_HPP__
#define __UTILITY_HPP__

#include <string>
#include <map>
#include <vector>


class Utility {
public:

    static  std::vector<std::string> split(std::string const &line, char del = ' ') {
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
};

#endif // __UTILITY_HPP__
