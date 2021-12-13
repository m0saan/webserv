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
//        os << "error_page: " << vec[i]._error_page << std::endl;
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
            os << "\t\tcgi: " << vec[i]._location[j]._cgi << std::endl;
            // os << "\t\tauto_index: " << vec[i]._location[j]._auto_index << std::endl;
        }

        std::cout << std::endl;
    }
    os << ']' << std::endl;
    return os;
}

int main(int ac, char **av) {
    if (ac != 2)
        exit(1);

    std::vector<ServerConfig> vect = performParsing(av[1]);
    Server serv(vect);
    serv.listen();

    return EXIT_SUCCESS;
}