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

void Chat::updateMsgs() const
{
    std::string msg1 = firstClient_->receiveInfo();
    if (!msg1.empty() && msg1 != "!leave")
    {
        secondClient_->sendInfo(msg1);
    }

    std::string msg2 = secondClient_->receiveInfo();
    if (!msg2.empty() && msg2 != "!leave")
    {
        firstClient_->sendInfo(msg2);
    }
    // TODO: have something happen when !leave is sent
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