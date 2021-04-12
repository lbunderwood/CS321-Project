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
    // move is okay, we just can't have duplicate Connection objects
    Chat():firstClient_(nullptr), secondClient_(nullptr) {throw;}
    Chat(const Chat& other) = delete;
    Chat& operator=(const Chat& other) = delete;
    Chat(Chat&& other) = default;
    Chat& operator=(Chat&& other) = default;

    // single-parameter constructor
    // takes a client pointer
    explicit Chat(std::shared_ptr<Connection> client);

private: // Private member functions for internal use



public: // Public member functions

    std::string recvName();

    void addSecondClient(std::shared_ptr<Connection> secondClient);

    bool updateMsgs() const;

    std::pair<std::string, std::string> getNames() const;

    std::shared_ptr<Connection> getClient() const;

    bool broadcast(const std::string& msg) const;

private: // Private member variables

    // the two clients involved (this may be expanded to allow group chats later)
    std::shared_ptr<Connection> firstClient_;
    std::shared_ptr<Connection> secondClient_;

    // the usernames chosen by the users -
    // both are assigned by the first user to connect, and they are used
    // to verify that the right person has been reached
    std::string firstUsrn_;
    std::string secondUsrn_;

};


#endif //CHAT_H
