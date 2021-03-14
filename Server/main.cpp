// Created 3/12/2021 by Luke Underwood
// CS321-Project
// main.cpp
// main function for client portion of DM project

#include <iostream>
#include <string>
#include "../Lib/Connection.h"

int main()
{
    int port = 58426;
    Connection clients("", port);

    std::cout << "Connection available, awaiting connection\n\n";

    while (true)
    {
        clients.acceptIncoming();
        for (auto n : clients.receiveInfo())
        {
            std::cout << n;
        }
    }
}