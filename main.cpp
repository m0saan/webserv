#include "includes/response.hpp"
#include "includes/location.hpp"
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include <dirent.h>

int main()
{
    std::vector<std::string> index;
    std::vector<std::string> allowed;
    index.push_back("hello.html");
    index.push_back("index.html");
    allowed.push_back("GET");
    Location loc("/", index, allowed);
    loc.getAutoIndex() = "on";
    Response res("/Users/mamoussa/Desktop/42/webserv", loc, "src", "/Users/mamoussa/Desktop/42/webserv/error_pages");
    res.Get_request();
// 	res.Post_request();
//    res.Delete_request();
    std::cout << res.get_response();
    // struct dirent *cur_dir;
    
    // DIR* dir = opendir("/Users/mamoussa/Desktop/42/webserv/src");
    // if (!dir)
    // {
    //     std::cout << "permission denied" << std::endl;
    //     return 0;
    // }
    // while ((cur_dir = readdir(dir)))
    // {
    //     std::cout << "d_ino \t" << cur_dir->d_ino << std::endl;
    //     std::cout << "d_name \t" << cur_dir->d_name << std::endl;
    //     std::cout << "d_namlen \t" << cur_dir->d_namlen << std::endl;
    //     std::cout << "d_reclen \t" << cur_dir->d_reclen << std::endl;
    //     std::cout << "d_seekoff \t" << cur_dir->d_seekoff << std::endl;
    //     std::cout << "d_type \t" << cur_dir->d_type << std::endl;
    //     std::cout << "---------------------------------------" << std::endl;
    // }
    return 0;
}