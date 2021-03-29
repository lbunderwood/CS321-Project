//
// Created by Luke U on 3/29/2021.
//

#include "Chat.h"

// three-parameter constructor
// takes a client and the two names that they have sent
Chat::Chat(Connection& client, const std::string& clientName, const std::string& otherName)
    : firstClient_(std::move(client)), firstUsrn_(clientName), secondUsrn_(otherName), secondClient_("", PORT) {}

void Chat::addSecondClient(Connection& secondClient)
{
    secondClient_ = std::move(secondClient);
}

void Chat::updateMsgs()
{
    std::string msg1 = firstClient_.receiveInfo();
    if (!msg1.empty() && msg1 != "!leave")
    {
        secondClient_.sendInfo(msg1);
    }

    std::string msg2 = secondClient_.receiveInfo();
    if (!msg2.empty() && msg2 != "!leave")
    {
        firstClient_.sendInfo(msg2);
    }
    // TODO: have something happen when !leave is sent
}