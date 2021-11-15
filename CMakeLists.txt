cmake_minimum_required(VERSION 3.20)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(webserv main.cpp includes/parser.hpp test.cpp includes/request.hpp includes/request.cpp includes/utility.hpp)
