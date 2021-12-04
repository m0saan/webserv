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

std::string getScriptName(std::string const& url) {
    std::size_t 
}

int main(int ac, char **av)
{
    if (ac != 2)
        exit(1);
    Request request;
    std::ifstream ifs;

    // std::cout << performParsing().size() << std::endl;
    // Server serv(performParsing(av[1]));
    // ifs.open("./request", std::ios_base::in);
    // if (ifs.is_open()) {
    //     std::stringstream ss;
    //     std::string line;
    //     while (std::getline(ifs, line))
    //         ss << line << std::endl;

    //     request.parseRequest(ss);
    //     auto it = request.getMap().begin();

    //     for(; it != request.getMap().end(); ++it) {
    //         std::cout << it->first << " ";
    //         for (int i = 0; i < it->second.size(); ++i)
    //             std::cout << it->second[i] << " ";
    //         std::cout << std::endl;
    //     }
    // }
    // serv.listen();
    char value_store[100];
    memset(value_store, 0, 100);
    std::string url("http://login:password@example.com/one/more/dir/file.exe?a=sth&b=sth");
    sscanf(url.c_str(), "%#*/", value_store);
    std::cout << value_store << std::endl;
    // std::map<std::string, std::string> queries;
    // std::pair<bool, int> has_queries_result;
    // has_queries_result = hasQueries(url);
    // if (has_queries_result.first)
    //     queries = getQueries(url, has_queries_result.second);

    // for (auto const p : queries)
    //     std::cout << p.first << ":" << p.second << std::endl;



    return EXIT_SUCCESS;
}
