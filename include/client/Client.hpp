#pragma once
#include <iostream>

using namespace std;

namespace client
{
    class Client
    {
    //-------------------------------------------------------------------------
    //      Attributes
    //-------------------------------------------------------------------------
    private:
        string username;
        string server;
        string port;


    //-------------------------------------------------------------------------
    //      Constructor
    //-------------------------------------------------------------------------
    private:
        Client(string username, string server, string port);


    //-------------------------------------------------------------------------
    //      Methods
    //-------------------------------------------------------------------------
    public:
        static void login(string username, string server, string port);
    };
}
