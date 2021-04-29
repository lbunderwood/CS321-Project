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
