cmake_minimum_required(VERSION 3.16)
project(Client)

set(CMAKE_CXX_STANDARD 14)

include_directories()

add_executable(Client main.cpp ../Lib/Connection.cpp ../Lib/Connection.h)