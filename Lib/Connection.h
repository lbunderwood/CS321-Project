//
// Created by Luke U on 3/11/2021.
// CS321-Project
// Connection class for handling network connections
//

#ifndef CONNECTION_H
#define CONNECTION_H

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <string>
#include <memory>
#include <map>
#include <vector>
#include <iostream>
#include <cstddef>

class Connection
{
public:     // Constructors and Destructor

    // Default constructor - not used
    Connection() = delete;

    // one-parameter constructor
    // takes a string specifying an address (empty string if self)
    // gets address info, socket descriptor, and binds to port
    Connection(std::string name);

    // two-parameter constructor
    // takes a socket descriptor and socket address storage
    // sets up a connection that was established via a call to acceptIncoming()
    // This should ONLY be used by acceptIncoming's call to make_shared, or if you
    // DEFINITELY know what you're doing.
    Connection(int sockDesc, sockaddr_storage sockAddr);

    // copying and moving are not allowed. pass pointers around if you must,
    // but re-binding new connections to the same remote host causes big issues
    Connection(const Connection& other) = delete;
    Connection& operator=(const Connection& other) = delete;
    Connection(Connection&& other) = delete;
    Connection& operator=(Connection&& other) = delete;

    // destructor - written explicitly to close network connection
    ~Connection();

private:

    // setup private member function
    // called to set up addrInfo_ and sockDesc_
    // called by constructor
    void setup();

public:     // Public member functions

    // connect public member function
    // should only be used by client programs
    // establishes a network connection
    // returns bool if successful
    bool connect() const;

    // acceptIncoming public member function
    // should only be used by server programs
    // establishes a connection with those who reach out
    // returns a pointer to the newly connected client Connection
    std::shared_ptr<Connection> acceptIncoming();

    // sendInfo public member function
    // sends information to connected host
    bool sendInfo(const std::string& info) const;

    // receiveInfo public member function
    // string of information from connected host
    std::string receiveInfo() const;

private:    // private member variables

    // holds the address of the host we're connecting to
    std::string address_;

    // the struct of information about our connection
    addrinfo* addrInfo_;

    // holds the socket descriptor for an outgoing connection
    int sockDesc_;

    // holds the client's socket address
    sockaddr_storage sockAddr_;

public:

    // port number
    const static int PORT = 58426;
};


#endif //CONNECTION_H
