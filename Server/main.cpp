// Created 3/12/2021 by Luke Underwood
// CS321-Project
// main.cpp
// main function for client portion of DM project

#include <iostream>
#include <string>
#include <vector>
#include "../Lib/Connection.h"
#include "Chat.h"

int main()
{
    // Array of connection pointers to delete later
    std::vector<std::shared_ptr<Connection>> connections;

    // Array of chats to update
    std::vector<Chat> activeChats;

    // Map of chats who do not yet have a second client
    std::map<std::string, Chat> openRequests;

    // open up a connection to accept any incoming connections
    std::shared_ptr<Connection> listenPort = std::make_shared<Connection>("");

    // Infinite loop. We don't want the server to just stop at any point
    while(true)
    {
        // open up a connection to accept any incoming connections
        std::shared_ptr<Connection> newClient = listenPort->acceptIncoming();
        // if we get a connection
        if(newClient != nullptr)
        {
            std::cout << "new connection opened\n\n";

            connections.push_back(newClient);
            // get their username and the name of the other person
            std::string theirName;
            std::string otherName;

            // this looping forever is prob a bad idea.
            // TODO: figure out how to use a thread to do this
            while(theirName.empty() || otherName.empty())
            {
                // put the first message that comes in as theirName
                if(theirName.empty())
                {
                    theirName = newClient->receiveInfo();
                    if(!theirName.empty())
                        std::cout << "First name received successfully.\n\n";
                }
                // put in the second one as the otherName
                else
                {
                    otherName = newClient->receiveInfo();
                }
            }
            std::cout << "Second name received successfully.\n\n";
            // if we have an open request for the person who just connected
            if(openRequests.count(theirName))
            {
                // add them to the chat
                openRequests[theirName].addSecondClient(newClient);
                // add the chat to activeChats
                activeChats.push_back(std::move(openRequests[theirName]));
                // remove them from openRequests
                openRequests.erase(theirName);
                // send a message to everyone to let the clients know they've been connected
                activeChats.back().broadcast("connection successful");
                std::cout << "second client connected. chat open for use.\n\n";
            }
            // otherwise, they connected first
            else
            {
                // create a new chat and add it to openRequests
                Chat newChat(newClient, theirName, otherName);
                openRequests.insert(std::make_pair(otherName, std::move(newChat)));
                std::cout << "first client connected. chat created\n\n";
            }
        }

        // update everyone's messages
        for(size_t i = 0; i < activeChats.size(); ++i)
        {
            activeChats[i].updateMsgs();
            if (i == activeChats.size() - 1)
            {
                std::cout << "messages updated!\n\n";
            }
        }
        std::cout << "its ya boi\n\n";
    }
}