#include "includes/response.hpp"
#include "includes/location.hpp"
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
    std::vector<std::string> index;
    index.push_back("hello.html");
    index.push_back("index.html");
    Location loc("/hello.html", index);
    Response res("/Users/mamoussa/Desktop/42/webserv", loc, "hello.html", "/Users/mamoussa/Desktop/42/webserv/error_pages");
    res.Get_request();
    std::cout << res.get_response();
    return EXIT_SUCCESS;
}



