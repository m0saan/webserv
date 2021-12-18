//
// Created by Mohammad  Boustta on 11/12/21.
//

#ifndef __UTILITY_HPP__
#define __UTILITY_HPP__

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>

#include "../includes/parser.hpp"
#include "request.hpp"
#include "utility.hpp"
#include "server.hpp"
#include <algorithm>

#define _MSG(msg)                                                                      \
    {                                                                                  \
        std::cerr << "file: " << __FILE__ << "(@" << __LINE__ << "): " << msg << '\n'; \
    }

class Utility
{
public:
    static std::vector<std::string> split(std::string const &line, char del = ' ')
    {
        std::vector<std::string> out;
        std::stringstream ss(line);
        std::string tmp;
        while (std::getline(ss, tmp, del))
        {
            if (tmp == " " || tmp.empty())
                continue;
            out.push_back(tmp);
        }
        return out;
    }

    static std::pair<bool, int> hasQueries(std::string const &url)
    {
        size_t res = url.find('?');
        return (std::make_pair(res != std::string::npos, res));
    }

    static std::string getQueries(std::string const &url, std::size_t querie_start)
    {
        return url.substr(querie_start + 1);
    }

    static std::string urlDecode(std::string &SRC)
    {
        std::string ret;
        char ch;
        int i, ii;
        for (i = 0; i < SRC.length(); i++)
        {
            if (int(SRC[i]) == 37)
            {
                sscanf(SRC.substr(i + 1, 2).c_str(), "%x", &ii);
                ch = static_cast<char>(ii);
                ret += ch;
                i = i + 2;
            }
            else
                ret += SRC[i];
        }
        return (ret);
    }

    static std::string getScriptName(std::string &url)
    {
        std::vector<std::string> res = Utility::split(url, '/');
        if (!res.empty())
            return res[res.size() - 1];
        return std::string("");
    }

    static bool startWith(std::string const &haystack, std::string const &needle)
    {
        for (size_t i = 0; i < needle.length() && i < haystack.size(); i++)
            if (needle.at(i) != haystack.at(i))
                return false;
        return true;
    }

    static ServerConfig getDefaultServerConfig()
    {
        ServerConfig default_server_config;
        default_server_config._port = "80";
        default_server_config._host = "localhost";
        default_server_config._server_name = "localhost";
        default_server_config._root = "./";
        default_server_config._max_file_size = "1048576";
        default_server_config._upload_store = "./";
        default_server_config._auto_index = "false";
        default_server_config._loc_path = "/";
        return default_server_config;
    }

    static ServerConfig
    getRightConfig(std::string const &port, std::string const &host, std::string const &server_name,
                   std::string const &url, std::vector<ServerConfig> const &config_vec)
    {
        std::vector<ServerConfig> possible_blocks;

        for (size_t i = 0; i < config_vec.size(); i++)
        {
            if (config_vec[i]._port == port && config_vec[i]._host == host)
                possible_blocks.push_back(config_vec[i]);
        }

        if (possible_blocks.size() > 1)
        {
            std::vector<ServerConfig>::iterator start = possible_blocks.begin();
            for (; start != possible_blocks.end();)
            {
                if (server_name.find(start->_server_name, 0) != std::string::npos)
                    ++start;
                else
                {
                    possible_blocks.erase(start);
                    start = possible_blocks.begin();
                }
            }
        }

        // request_url.starts_with(location_url)  -> right location
        if (possible_blocks.empty())
        {
            // std::cout << "default" << std::endl;
            return getDefaultServerConfig();
        }

        ServerConfig loc;
        ServerConfig default_loc;
        loc._loc_path = default_loc._loc_path = "";

        for (size_t j = 0; j < possible_blocks[0]._location.size(); ++j)
        {
            if (possible_blocks[0]._location[j]._loc_path == "/")
            {
                default_loc = possible_blocks[0]._location[j];
                continue;
            }
            if (url.find(possible_blocks[0]._location[j]._loc_path) != std::string::npos)
            {
                loc = loc._loc_path.length() < possible_blocks[0]._location[j]._loc_path.length()
                          ? possible_blocks[0]._location[j]
                          : loc;
            }
        }

        /*
         * Location:
         * - root
         * - index
         * - auto index
         * - cgi
         * - redirect

         */

        // This would choose the default location block.
        if (url == "/" || loc._loc_path.empty())
            loc = default_loc;

        // If no location provided!, create a default one.
        if (loc._loc_path.empty() && default_loc._loc_path.empty())
        {
            loc._loc_path = "/";
        }

        // This is the configuration that will be sent to the response handler.
        possible_blocks[0]._cgi = loc._cgi;
        possible_blocks[0]._redirect = loc._redirect;
        possible_blocks[0]._auto_index = loc._auto_index;
        possible_blocks[0]._loc_path = loc._loc_path;
        possible_blocks[0]._upload_store = loc._upload_store;
        if (loc._allowed_method.size() > 0)
            possible_blocks[0]._allowed_method = loc._allowed_method;
        if (loc._index.size() > 0)
            possible_blocks[0]._index = loc._index;
        if (!loc._root.empty())
            possible_blocks[0]._root = loc._root;
        if (!loc._max_file_size.empty())
            possible_blocks[0]._max_file_size = loc._max_file_size;

        return possible_blocks[0];
    }
};

#endif // __UTILITY_HPP__
