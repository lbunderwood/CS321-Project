//
// Created by Luke U on 3/29/2021.
//

#include "Chat.h"

// three-parameter constructor
// takes a client and the two names that they have sent
Chat::Chat(std::shared_ptr<Connection> client)
    : firstClient_(std::move(client)), secondClient_(nullptr) {}

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

void Chat::addSecondClient(std::shared_ptr<Connection> secondClient)
{
    secondClient_ = std::move(secondClient);
}

int Chat::updateMsgs() const
{
    bool output = -1;
    std::string msg1 = firstClient_->receiveInfo();

    if(msg1.empty())
    {
        output = 0;
    }
    else if (msg1 != "\n\n" && msg1 != "!leave")
    {
        secondClient_->sendInfo(msg1);
        output = 1;
    }

    std::string msg2 = secondClient_->receiveInfo();
    if(msg2.empty())
    {
        output = 0;
    }
    else if (msg2 != "\n\n" && msg1 != "!leave")
    {
        firstClient_->sendInfo(msg2);
        output = 1;
    }

    return output;
}

std::pair<std::string, std::string> Chat::getNames() const
{
    return {firstUsrn_, secondUsrn_};
}

std::shared_ptr<Connection> Chat::getClient() const
{
    return firstClient_;
}

bool Chat::broadcast(const std::string& msg) const
{
    return firstClient_->sendInfo(msg) && secondClient_->sendInfo(msg);
}