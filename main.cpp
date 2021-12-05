#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>

#include "parser/parser.hpp"
#include "includes/request.hpp"
#include "includes/utility.hpp"
#include "includes/server.hpp"

std::ostream &operator<<(std::ostream &os, std::vector<std::string> const &vec)
{
    os << "[ ";
    for (int i = 0; i < vec.size(); ++i)
        os << vec[i] << " ";
    os << ']' << std::endl;
    return os;
}

std::ostream &operator<<(std::ostream &os, std::vector<ServerConfig> const &vec)
{
    os << "[" << std::endl;
    for (int i = 0; i < vec.size(); ++i)
    {
        std::cout << "--------------------- ServerConfig " << i + 1 << " ---------------------" << std::endl;

        os << "port: " << vec[i]._port << std::endl;
        os << "host: " << vec[i]._host << std::endl;
        os << "server_name: " << vec[i]._server_name << std::endl;
        os << "error_page: " << vec[i]._error_page << std::endl;
        os << "max_file_size: " << vec[i]._max_file_size << std::endl;
        os << "time_out: " << vec[i]._time_out << std::endl;

        for (size_t j = 0; j < vec[i]._location.size(); j++)
        {
            os << "\tlocation" << std::endl;
            os << "\t\tpath: " << vec[i]._location[j]._loc_path << std::endl;
            os << "\t\troot: " << vec[i]._location[j]._root << std::endl;
            os << "\t\tallowed_method: ";
            for (std::set<std::string>::iterator ut = vec[i]._location[j]._allowed_method.begin();
                 ut != vec[i]._location[j]._allowed_method.end(); ++ut)
                os << *ut << " ";
            os << std::endl;
            os << "\t\tindex: " << vec[i]._location[j]._index << std::endl;
            os << "\t\tauto_index: " << vec[i]._location[j]._auto_index << std::endl;
        }

        std::cout << std::endl;
    }
    os << ']' << std::endl;
    return os;
}

ServerConfig &getRightConfig(std::string const& port, std::string const& host, std::string const& server_name,
                                    std::string const& url,std::vector<ServerConfig> const &config_vec)
{
    std::vector<ServerConfig> possible_blocks;
    int loc_index;
    for (size_t i = 0; i < config_vec.size(); i++)
    {
        if (config_vec[i]._port == port && config_vec[i]._host == host)
            possible_blocks.push_back(config_vec[i]);
    }


    if (possible_blocks.size() > 1)
    {
        for (size_t i = 0; i < config_vec.size(); i++)
        {
            if (server_name.find(config_vec[i]._server_name, 0) != std::string::npos)
                continue;
            else
                possible_blocks.erase(possible_blocks.begin() + i);
        }
    }

    // request_url.starts_with(location_url)  -> right location
    ServerConfig loc();
    for (size_t i = 0; i < config_vec.size(); i++)
    {
        for (size_t j = 0; j < config_vec[i]._location.size(); ++j)
            if (Utility::startWith(url, config_vec[i]._location[j]._loc_path))
                loc = loc._loc_path.length() <  config_vec[i]._location[j]._loc_path.length() ? config_vec[i]._location[j] : loc;
    }

    return possible_blocks[0];
}

int main(int ac, char **av)
{
    if (ac != 2)
        exit(1);

    std::vector<ServerConfig> res = performParsing(av[1]);

    Server ser(res);

    ser.listen();


    // std::cout << res << std::endl;
    // TODO: check the config file directive non of them is empty.
    return EXIT_SUCCESS;
}

/*
    std::string url("http://www.example.com/index234.html");
    std::vector<std::string> queries;
    std::pair<bool, int> has_queries_result;
    has_queries_result = Utility::hasQueries(url);
    if (has_queries_result.first)
        queries = Utility::getQueries(url, has_queries_result.second);
    std::string script_name = Utility::getScriptName(url);

    for (size_t i = 0; i < queries.size(); i++)
        std::cout << queries[i] << std::endl;
    std::cout << std::endl << script_name << std::endl;

*/