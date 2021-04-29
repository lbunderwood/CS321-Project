// Created 3/12/2021 by Luke Underwood
// CS321-Project
// main.cpp
// main function for client portion of DM project

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "../Lib/Connection.h"
#include "Chat.h"

int main()
{
    // Array of chats to update
    std::vector<Chat> activeChats;

    // Array of chats that are waiting for a match
    std::vector<Chat> openRequests;

    // Map of usernames that the Chats in openRequests are looking for
    std::map<std::string, Chat*> chatNames;

    // Listening port to accept any incoming connections
    std::shared_ptr<Connection> listenPort = std::make_shared<Connection>("");

    // Infinite loop. We don't want the server to just stop at any point
    while(true)
    {
        // open up a connection to accept any incoming connections
        std::shared_ptr<Connection> newClient = listenPort->acceptIncoming();

        // if we get a connection
        if(newClient != nullptr)
        {
            std::cout << "new client connected!\n\n";
            Chat newChat(newClient);
            openRequests.push_back(std::move(newChat));
        }

        // iterate through clients who are not yet in a chat and see if they've messaged us.
        // if they have, add them to chats or to chatNames when appropriate
        for(std::size_t i = 0; i < openRequests.size(); ++i)
        {
            // names received so far from current client
            auto names = openRequests[i].getNames();

            // if they've yet to send both names
            if(names.second.empty())
            {
                // check to see if they've sent a name yet
                auto newName = openRequests[i].recvName();

                // check for a disconnection
                if(newName == "\n\n")
                {
                    openRequests.erase(openRequests.begin() + i);
                    std::cout << "Someone disconnected!\n\n";
                }

                // if they sent us the name of the person they'd like to connect to
                if(!names.first.empty() && !newName.empty())
                {
                    std::cout << "A name was received!\n\n";
                    // if their name and the person they'd like to connect to correspond to
                    // a chat we already have open
                    if(chatNames.count(names.first) &&
                       chatNames[names.first]->getNames().first == newName)
                    {
                        // add them to the other chat
                        chatNames[names.first]->addSecondClient(openRequests[i].getClient());
                        // add the chat to activeChats
                        activeChats.push_back(std::move(*chatNames[names.first]));
                        // erase both chats from openRequests
                        // This is really unfortunate, as this linear-time operation makes this
                        // for-loop O(n^2), but any data structure with more efficient random-access
                        // deletion has other restrictions that prevent this loop from performing
                        // everything it needs to :(
                        openRequests.erase(openRequests.begin() + i);
                        openRequests.erase(openRequests.begin() + std::distance(openRequests.data(), chatNames[names.first]));
                        // remove the chat from chatNames
                        chatNames.erase(names.first);
                        // let them know they've been connected
                        activeChats.back().broadcast("connection successful");
                        std::cout << "Chat created!\n\n";
                    }
                    else // if they connected first, add them to chatNames
                    {
                        chatNames.insert({newName, &openRequests[i]});
                    }
                }
            }
        }

        // update everyone's messages
        for(size_t i = 0; i < activeChats.size(); ++i)
        {
            int result = activeChats[i].updateMsgs();
            if(result == 1)
            {
                std::cout << "A message was sent!\n\n";
            }
            else if(result == -1)
            {
                activeChats.erase(activeChats.begin() + i);
                std::cout << "Someone disconnected!\n\n";
            }
        }
    }
}