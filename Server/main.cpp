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

    while(true)
    {
        clients.acceptIncoming();
        auto msgs = clients.receiveInfo();
        for (auto n : msgs)
        {
            std::cout << "\n" << n << "\n";
        }
    }
}