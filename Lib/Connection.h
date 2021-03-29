//
// Created by Luke U on 3/11/2021.
// CS321-Project
// Connection class for handling network connections
//

#ifndef CONNECTION_H
#define CONNECTION_H

#include <sys/types.h>
#include <sys/socket.h>
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

    // two-parameter constructor
    // takes a string specifying an address (empty string if self), and a port number
    // gets address info, socket descriptor, and binds to port
    Connection(std::string name, int port);

    // copy and move constructors and assignment operators will not be used,
    // primarily because I do not know how that would work with the network stuff
    // if you need a connection, use the instance of the class that you have.
    // if you want to make new connections, just make a new instance of the class
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
    bool acceptIncoming();

    // sendInfo public member function
    // sends information to connected host
    bool sendInfo(const std::string& info) const;

    // receiveInfo public member function
    // string of information from connected host
    std::string receiveInfo() const;

private:    // private member variables

    // holds the address of the host we're connecting to
    std::string address_;

    // holds the port number
    int portNum_;

    // the struct of information about our connection
    std::unique_ptr<addrinfo> addrInfo_;

    // holds the socket descriptor for an outgoing connection
    int sockDesc_;

    // holds the client's socket address
    sockaddr_storage sockAddr_;
};


#endif //CONNECTION_H
