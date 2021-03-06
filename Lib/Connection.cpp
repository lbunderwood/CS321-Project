//
// Created by Luke U on 3/11/2021.
//

#include "Connection.h"

// one-parameter constructor
// takes a string specifying an address (empty string if self)
// gets address info, socket descriptor, and binds to port
Connection::Connection(std::string name)
    : addrInfo_(nullptr), address_(std::move(name)), sockDesc_(0)
{
    setup();
}

// two-parameter constructor
// takes a socket descriptor and socket address storage
// sets up a connection that was established via a call to acceptIncoming()
Connection::Connection(int sockDesc, sockaddr_storage sockAddr)
    : sockDesc_(sockDesc), sockAddr_(sockAddr), addrInfo_(nullptr), address_("") {}

//destructor - written explicitly to close network connection
Connection::~Connection()
{
    if(close(sockDesc_) == -1)
    {
        perror("Error in closing connection");
    }
    freeaddrinfo(addrInfo_);
}

// setup private member function
// called to set up addrInfo_ and sockDesc_
// called by constructor
void Connection::setup()
{
    addrinfo hints{};   // a struct of information to pass to getaddrinfo

    memset(&hints, 0, sizeof hints); // set all bits in hints to 0
    hints.ai_family = AF_INET;        // we want to stay flexible on our ip version
    hints.ai_socktype = SOCK_STREAM;    // we will use TCP stream sockets, rather than datagram sockets
    if (address_.empty()) hints.ai_flags = AI_PASSIVE;        // if this is a server, set AI_PASSIVE flag

    // we want a c++ outward interface, but we must convert for getaddrinfo
    const char* cName = address_.c_str();
    if (address_.empty())
    {
        cName = nullptr;
    }

    // get address info, store status in status, store info in *servinfo, and handle errors
    int status = getaddrinfo(cName, std::to_string(PORT).c_str(), &hints, &(addrInfo_));
    if (status != 0)
    {
        perror("getaddrinfo call failed.");
        std::cout << "getaddrinfo error: \n" << gai_strerror(status) << std::endl;
        throw;
    }

    // get a socket descriptor
    sockDesc_ = socket(addrInfo_->ai_family, addrInfo_->ai_socktype | SOCK_NONBLOCK, addrInfo_->ai_protocol);
    // handle errors
    if (sockDesc_ == -1)
    {
        perror("Call to socket failed");
        throw;
    }

    // bind the socket descriptor to the port, mark this port as listening, and handle errors
    // but only if we're the server
    if (address_.empty())
    {
        if (bind(sockDesc_, addrInfo_->ai_addr, addrInfo_->ai_addrlen) == -1)
        {
            perror("Call to bind failed");
            throw;
        }

        // listen for connections
        int maxConnections = 10;
        if(listen(sockDesc_, maxConnections) == -1)
        {
            perror("Call to listen failed");
        }
    }
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
std::shared_ptr<Connection> Connection::acceptIncoming()
{
    // accept a connection
    sockaddr_storage clientAddr{};
    socklen_t addr_size = sizeof(clientAddr);
    int sockDesc = accept4(sockDesc_, (sockaddr *)&clientAddr, &addr_size, SOCK_NONBLOCK);
    if (sockDesc == -1)
    {
        if(errno != EAGAIN && errno != EWOULDBLOCK)
        {
            perror("Call to accept failed");
        }
        return nullptr;
    }
    return std::make_shared<Connection>(sockDesc, clientAddr);
}

// sendInfo function
// sends information to connected host
bool Connection::sendInfo(const std::string& info) const
{
    // set up variables to pass to send
    auto cInfo = info.c_str();
    std::size_t length = strlen(cInfo);
    std::size_t sent = 0;

    while (sent < length)
    {
        int result = send(sockDesc_, (const void*)&cInfo[sent], length - sent, 0);
        if (result == -1)
        {
            perror("Call to send failed");
            return false;
        }
        sent += result;
    }

    return true;
}

// receiveInfo function
// returns string of information from connected host
// if host disconnects, "\n\n" is returned to signal this,
// because it is something that the clients are incapable of sending
std::string Connection::receiveInfo() const
{
    // set up variables
    int maxSize = 512;
    char* cInfo = new char[maxSize];

    // receive some information and do error handling
    int result = recv(sockDesc_, (void*)cInfo, maxSize, MSG_DONTWAIT);
    if (result == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
    {
        perror("Call to recv failed");
        return "";
    }
    else if (result == 0)   // recv returns 0 on disconnect
    {
        return "\n\n";
    }

    // return what we received
    std::string output(cInfo);
    delete[] cInfo;
    return output;
}