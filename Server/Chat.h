// Chat.h
// Created by Luke Underwood
// Created 2021-03-29
// holds Chat class, which manages two connected clients and the communication between them

#ifndef CHAT_H
#define CHAT_H

#include <utility>
#include "../Lib/Connection.h"

class Chat
{
public: // Constructors and destructor

    // no copy or default construction allowed
    // move is okay and it written by the compiler,
    // we just can't have duplicate Connection objects
    Chat() = delete;
    Chat(const Chat& other) = delete;
    Chat& operator=(const Chat& other) = delete;
    Chat(Chat&& other) = default;
    Chat& operator=(Chat&& other) = default;

    // single-parameter constructor
    // takes a client pointer
    // not sure why my linter says this isn't used... it's used in main
    explicit Chat(std::shared_ptr<Connection> client);

private: // Private member functions for internal use

    // private member function recvSend
    // receives information from "first" and sends it to "second"
    // returns a 1 if a message was sent, a 0 if not, and a -1 if someone disconnected
    int recvSend(std::shared_ptr<Connection> first, std::shared_ptr<Connection> second) const;

public: // Public member functions

    // member function recvName
    // takes input from firstClient_ and assigns usernames
    // returns a name if one was received, empty string if not
    std::string recvName();

    // member function addSecondClient
    // takes a pointer to the second client and sets secondClient_ to it
    void addSecondClient(std::shared_ptr<Connection> secondClient);

    // member function updateMsgs
    // calls recvSend twice to send messages both ways between clients
    // returns -1 if either client disconnected, returns 1 if either sent a message,
    // returns 0 if neither sent anything or disconnected. -1 takes highest precedence
    int updateMsgs() const;

    // member function getNames
    // returns a pair containing {firstUsrn_, secondUsrn_}
    std::pair<std::string, std::string> getNames() const;

    // member function getClient
    // returns a pointer to firstClient_
    // used in conjunction with addSecondClient to combine two chats
    std::shared_ptr<Connection> getClient() const;

    // member function broadcast
    // used to send information to both clients at once
    bool broadcast(const std::string& msg) const;

private: // Private member variables

    // the two clients involved
    std::shared_ptr<Connection> firstClient_;
    std::shared_ptr<Connection> secondClient_;

    // the usernames chosen by the users -
    // both are assigned by the first user to connect, and they are used
    // to verify that the right person has been reached
    std::string firstUsrn_;
    std::string secondUsrn_;

};


#endif //CHAT_H
