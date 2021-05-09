#pragma once
#include <iostream>
#include <unistd.h>

/// <summary>
///     Responsible for representing a server.
/// </summary
class Server
{
//-------------------------------------------------------------------------
//		Attributes
//-------------------------------------------------------------------------
private:
    std::string ip;
    uint16_t port;
    pid_t pid;


//-------------------------------------------------------------------------
//		Constructor
//-------------------------------------------------------------------------
public:
    Server(std::string ip, uint16_t port, pid_t pid);


//-------------------------------------------------------------------------
//		Methods
//-------------------------------------------------------------------------
public:
    std::string get_ip();
    uint16_t get_port();
    std::string get_signature();
    pid_t get_pid();


//-------------------------------------------------------------------------
//		Operators
//-------------------------------------------------------------------------
    friend bool operator == (const Server &lhs, const Server &rhs) 
    {
        return (lhs.ip == rhs.ip) && (lhs.port == rhs.port);
    }

    friend bool operator != (const Server &lhs, const Server &rhs) 
    {
        return !(lhs.ip == rhs.ip) && (lhs.port == rhs.port);
    }

    friend std::ostream& operator << (std::ostream& outs, const Server &s)
    {
        return outs << "Server [" << s.ip << "; " << s.port << "]";
    }
};