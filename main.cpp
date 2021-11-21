#include "includes/response.hpp"
#include "includes/location.hpp"
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
    std::vector<std::string> index;
    std::vector<std::string> allowed;
    index.push_back("hello.html");
    index.push_back("index.html");
    allowed.push_back("GET");
    Location loc("/", index, allowed);
    Response res("/Users/mamoussa/Desktop/42/webserv", loc, "", "/Users/mamoussa/Desktop/42/webserv/error_pages");
    res.Get_request();
    std::cout << res.get_response();
    return EXIT_SUCCESS;
}



