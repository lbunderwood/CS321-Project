// Created 3/12/2021 by Luke Underwood
// CS321-Project
// main.cpp
// main function for client portion of DM project

#include <iostream>
#include <string>
#include "../Lib/Connection.h"

int main(int argc, char** argv)
{
    int port = 58426;

    // Print welcome message and collect info from user
    std::cout << "Hello! Welcome to Underwood Direct Message Service!\n\n"
                 << "Please enter the IPv4 address of the server you'd like to connect to: ";
    std::string servAddr;
    std::getline(std::cin, servAddr);
    std::cout << "\nPlease enter your username: ";
    std::string myName;
    std::getline(std::cin, myName);
    std::cout << "\nPlease enter the username of the person you'd like to talk with: ";
    std::string theirName;
    std::getline(std::cin, theirName);

    // establish server connection and send an ip you'd like to connect with
    Connection server(servAddr, port);
    server.connect();
    server.sendInfo(myName);
    server.sendInfo(theirName);

    // hang until connection is established
    std::cout << "\nWaiting for the other user...\n";
    while(server.receiveInfo() != "connection successful") {}

    // give the user some instructions
    std::cout << "Connection successful!\n"
                 << "Type a message and press enter to send, "
                 << "hit enter to check for incoming messages,"
                 << "or type \"!leave\" to disconnect!\n\n";

    // loop until someone disconnects -
    // the local user will use !leave command to disconnect, while the server
    // will send "\n\n" to disconnect, as this is a message that the remote user
    // cannot possibly send.
    std::string outMsg = "";
    std::string inMsg = "";
    while(outMsg != "!leave" && inMsg != "\n\n")
    {
        // get user input
        std::cout << myName << " > ";
        std::getline(std::cin, outMsg);
        if(!outMsg.empty())
        {
            server.sendInfo(outMsg);
        }

        // see if we have any messages
        inMsg = server.receiveInfo();
        if(inMsg != "\n\n")
        {
            std::cout << theirName << " > " << inMsg;
        }
    }
}

