#pragma once
#include <iostream>
#include <list>
#include "client/models/Notification.hpp"

using namespace std;

namespace client::models
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
        static list<models::Notification*> getNotifications();
    };
}
