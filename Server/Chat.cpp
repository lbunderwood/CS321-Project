//
// Created by Luke U on 3/29/2021.
//

#include "Chat.h"

// single-parameter constructor
// takes a client pointer
// not sure why my linter says this isn't used... it's used in main
Chat::Chat(std::shared_ptr<Connection> client)
    : firstClient_(std::move(client)), secondClient_(nullptr) {}

// member function recvName
// takes input from firstClient_ and assigns usernames
// returns a name if one was received, empty string if not
std::string Chat::recvName()
{
    std::string name = firstClient_->receiveInfo();
    if(name.empty()) return name;

    if(firstUsrn_.empty())
    {
        firstUsrn_ = name;
        return name;
    }
    else if(secondUsrn_.empty())
    {
        secondUsrn_ = name;
        return name;
    }
    else return "";
}

// member function addSecondClient
// takes a pointer to the second client and sets secondClient_ to it
void Chat::addSecondClient(std::shared_ptr<Connection> secondClient)
{
    secondClient_ = std::move(secondClient);
}

// member function updateMsgs
// calls recvSend twice to send messages both ways between clients
// returns -1 if either client disconnected, returns 1 if either sent a message,
// returns 0 if neither sent anything or disconnected. -1 takes highest precedence
int Chat::updateMsgs() const
{
    int output1 = recvSend(firstClient_, secondClient_);
    int output2 = recvSend(secondClient_, firstClient_);

    if(output1 == 0 && output2 == 0)
    {
        return 0;
    }
    else if(output1 == -1 || output2 == -1)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

// member function getNames
// returns a pair containing {firstUsrn_, secondUsrn_}
std::pair<std::string, std::string> Chat::getNames() const
{
    return {firstUsrn_, secondUsrn_};
}

// member function getClient
// returns a pointer to firstClient_
// used in conjunction with addSecondClient to combine two chats
std::shared_ptr<Connection> Chat::getClient() const
{
    return firstClient_;
}

// member function broadcast
// used to send information to both clients at once
bool Chat::broadcast(const std::string& msg) const
{
    return firstClient_->sendInfo(msg) && secondClient_->sendInfo(msg);
}

// private member function recvSend
// receives information from "first" and sends it to "second"
// returns a 1 if a message was sent, a 0 if not, and a -1 if someone disconnected
int Chat::recvSend(std::shared_ptr<Connection> first, std::shared_ptr<Connection> second) const
{
    int output = 0;
    std::string msg = first->receiveInfo();

    if(msg == "\n\n" || msg == "!leave")
    {
        return -1;
    }
    else if (!msg.empty())
    {
        second->sendInfo(msg);
        return 1;
    }
    else
    {
        return 0;
    }
}
