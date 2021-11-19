#include "includes/response.hpp"
#include "includes/location.hpp"

int main()
{
    std::vector<std::string> index;
    index.push_back("file.html");
    index.push_back("index.html");
    Location loc("/", index);
    Response res;

    res.Get_request("/Users/mamoussa/Desktop/42/webserv", loc, "");
    if (!res.get_error())
        std::cout << res.get_response();
    else
        std::cout << "file not found" << std::endl;
    return EXIT_SUCCESS;
}



