// Created by Luke U on 1/22/2021.
// CS321-Project
// main.cpp
// source file for main function

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <cstring>
#include <string>

int main()
{
    // This first chunk of code is largely copied from Beej's Guide to Network Programming
    // found here https://beej.us/guide/bgnet/html/ to get me set up. I have written my own
    // comments to display understanding

    int status;         // will hold result of getaddrinfo
    const char* portNum = "58426";
    addrinfo hints{};   // a struct of information to pass to getaddrinfo
    addrinfo *servinfo; // the titular info that will be gotten by getaddrinfo

    memset(&hints, 0, sizeof hints); // set all bits in hints to 0
    hints.ai_family = AF_UNSPEC;        // we want to stay flexible on our ip version
    hints.ai_socktype = SOCK_STREAM;    // we will use TCP stream sockets, rather than datagram sockets
    hints.ai_flags = AI_PASSIVE;        // set my IP automatically

    // get address info, store status in status, store info in *servinfo
    status = getaddrinfo(nullptr, portNum, &hints, &servinfo);
    // handle errors
    if (status != 0)
    {
        std::cout << "getaddrinfo error: \n" << gai_strerror(status) << std::endl;
        exit(1);
    }

    // get a socket descriptor
    int sockDesc = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    // handle errors
    if (sockDesc == -1)
    {
        std::cout << "error in obtaining socket descriptor";
        exit(1);
    }

    freeaddrinfo(servinfo);
    return 0;
}

