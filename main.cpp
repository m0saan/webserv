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

std::ostream &operator<<(std::ostream &os, std::vector<std::string> const &vec) {
    os << "[ ";
    for (int i = 0; i < vec.size(); ++i)
        os << vec[i] << " ";
    os << ']' << std::endl;
    return os;
}

std::ostream &operator<<(std::ostream &os, std::vector<ServerConfig> const &vec) {
    os << "[" << std::endl;
    for (int i = 0; i < vec.size(); ++i) {
        std::cout << "--------------------- ServerConfig " << i + 1 << " ---------------------" << std::endl;

        os << "port: " << vec[i]._port << std::endl;
        os << "host: " << vec[i]._host << std::endl;
        os << "server_name: " << vec[i]._server_name << std::endl;
        os << "error_page: " << vec[i]._error_page << std::endl;
        os << "max_file_size: " << vec[i]._max_file_size << std::endl;
        os << "time_out: " << vec[i]._time_out << std::endl;

        for (size_t j = 0; j < vec[i]._location.size(); j++) {
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


int main(int ac, char **av)
{
    if (ac != 2)
        exit(1);

    std::vector<ServerConfig> res = performParsing(av[1]);
    // TODO: check the config file directive non of them is empty.
    
    Server serv(res);
    serv.listen();
    // char value_store[100];
    // memset(value_store, 0, 100);
    // std::string url("http://login:password@example.com/one/more/dir/file.exe?a=sth&b=sth");
    // sscanf(url.c_str(), "%#*/", value_store);
    // std::cout << value_store << std::endl;
    // std::map<std::string, std::string> queries;
    // std::pair<bool, int> has_queries_result;
    // has_queries_result = hasQueries(url);
    // if (has_queries_result.first)
    //     queries = getQueries(url, has_queries_result.second);

    // for (auto const p : queries)
    //     std::cout << p.first << ":" << p.second << std::endl;



    return EXIT_SUCCESS;
}
