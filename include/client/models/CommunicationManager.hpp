#pragma once
#include <iostream>

using namespace std;

namespace client
{
    class CommunicationManager
    {
    //-------------------------------------------------------------------------
    //      Constructor
    //-------------------------------------------------------------------------
    private:
        CommunicationManager();


    //-------------------------------------------------------------------------
    //      Methods
    //-------------------------------------------------------------------------
    public:
        static bool login(string username, string server, string port);
        static void follow(string follower, string followed);
        static void sendNotification(string profile, string message);
    };
}
