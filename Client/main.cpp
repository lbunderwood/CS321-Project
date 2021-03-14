// Created 3/12/2021 by Luke Underwood
// CS321-Project
// main.cpp
// main function for client portion of DM project

#include <iostream>
#include <string>
#include "../Lib/Connection.h"

int main(int argc, char** argv)
{
    std::string address;
    int port = 0;
    if (argc == 3)
    {
        address = argv[1];
        port = std::stoi(argv[2]);
    }

    std::cout << "Arguments processed successfully!\n\n";

    Connection server(address, port);
    server.connect();
    server.sendInfo("Hello, world!");

    std::cout << "Message sent, program complete.\n\n";
    while(true){} // hang
}

