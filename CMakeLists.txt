cmake_minimum_required(VERSION 3.20)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(webserv ./fork.cpp
        ./includes/request.cpp
        ./includes/request_response.cpp
        ./includes/server.cpp
        ./includes/sockets.cpp
        ./main.cpp
        ./parser/parser.cpp
        ./src/response.cpp
        ./test.cpp)
