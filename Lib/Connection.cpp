//
// Created by Luke U on 3/11/2021.
//

#include "Connection.h"

// two-parameter constructor
// takes a string specifying an address (empty string if self), and a port number
// gets address info, socket descriptor, and binds to port
Connection::Connection(std::string name, int port) : address_(std::move(name)), portNum_(port), sockDesc_(0)
{
    addrInfo_ = std::make_unique<addrinfo>();
    setup();
}

//destructor - written explicitly to close network connection
Connection::~Connection()
{
    freeaddrinfo(addrInfo_.get());
}

void Connection::setup()
{
    addrinfo hints{};   // a struct of information to pass to getaddrinfo

    memset(&hints, 0, sizeof hints); // set all bits in hints to 0
    hints.ai_family = AF_UNSPEC;        // we want to stay flexible on our ip version
    hints.ai_socktype = SOCK_STREAM;    // we will use TCP stream sockets, rather than datagram sockets
    hints.ai_flags = AI_PASSIVE;        // set my IP automatically

    // we want a c++ outward interface, but we must convert for getaddrinfo
    const char* cName = address_.c_str();
    if (address_.empty())
    {
        cName = nullptr;
    }

    // get address info, store status in status, store info in *servinfo, and handle errors
    addrinfo* rawAddr = addrInfo_.get();
    int status = getaddrinfo(cName, std::to_string(portNum_).c_str(), &hints, &(rawAddr));
    if (status != 0)
    {
        std::cout << "getaddrinfo error: \n" << gai_strerror(status) << std::endl;
        throw;
    }

    // get a socket descriptor
    sockDesc_ = socket(rawAddr->ai_family, rawAddr->ai_socktype, rawAddr->ai_protocol);
    // handle errors
    if (sockDesc_ == errno)
    {
        std::cout << "error in obtaining socket descriptor";
        throw;
    }

    // bind the socket descriptor to the port and handle errors
    if(bind(sockDesc_, addrInfo_->ai_addr, addrInfo_->ai_addrlen) == errno)
    {
        std::cout << "error in binding socket file descriptor to port";
        throw;
    }
}

bool Connection::sendUntilComplete(const std::string& info, int socket)
{
    // set up variables to pass to send
    auto cInfo = info.c_str();
    std::size_t length = strlen(cInfo);
    std::size_t sent = 0;

    while (length > sent)
    {
        int result = send(socket, (const void*)&cInfo[sent], length - sent, 0);
        if (result == errno)
        {
            std::cout << "error in sending information to server";
            return false;
        }
        sent += result;
    }
    return true;
}

std::string Connection::recvString(int socket)
{
    // set up variables to pass to send
    const char* cInfo = "";
    int maxSize = 512;
    int result = 1;

    result = recv(socket, (void*)cInfo, maxSize, 0);
    if (result == errno)
    {
        std::cout << "error in sending information to server";
        return "";
    }
    else if (result == 0)
    {
        return "";// TODO: write code to close connection
    }

    return cInfo;
}

// connect public member function
// should only be used by client programs
// establishes a network connection
// returns bool if successful
bool Connection::connect() const
{
    return ::connect(sockDesc_, addrInfo_->ai_addr, addrInfo_->ai_addrlen) != errno;
}

// acceptIncoming public member function
// should only be used by server programs
// establishes a connection with those who reach out
bool Connection::acceptIncoming()
{
    // listen for connections
    int maxConnections = 10;
    if(listen(sockDesc_, maxConnections) == errno)
    {
        std::cout << "error in listening for incoming connections";
        return false;
    }

    // accept connections, store socket field descriptors in sockets_
    sockaddr_storage clientAddr{};
    socklen_t addr_size = sizeof(clientAddr);
    int sock = accept(sockDesc_, (sockaddr *)&clientAddr, &addr_size);
    if (sock == errno)
    {
        std::cout << "error in accepting an incoming connection";
        return false;
    }
    sockets_.insert({ sock, clientAddr });
    return true;
}

// sendInfo function
// sends information to connected host
bool Connection::sendInfo(const std::string& info) const
{
    // either send to the server, or to all the hosts
    if (sockets_.empty())
    {
        if (!sendUntilComplete(info, sockDesc_)) return false;
    }
    else
    {
        for (auto n : sockets_)
        {
            if (!sendUntilComplete(info, n.first)) return false;
        }
    }
    return true;
}

// receiveInfo function
// returns string of information from connected host
std::vector<std::string> Connection::receiveInfo() const
{
    // receive from the server if we are a client
    if (sockets_.empty())
    {
        return { recvString(sockDesc_) };
    }

    // receive from all clients if we are the server
    std::vector<std::string> output;
    for (auto n : sockets_)
    {
        output.push_back(recvString(n.first));
    }
    return output;
}