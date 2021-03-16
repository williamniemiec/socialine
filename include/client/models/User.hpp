#pragma once
#include <iostream>
#include "client/models/NotificationManager.hpp"

using namespace std;

namespace client::models
{
    class User
    {
    //-------------------------------------------------------------------------
    //      Attributes
    //-------------------------------------------------------------------------
    private:
        string username;
        models::NotificationManager* notificationManager;


    //-------------------------------------------------------------------------
    //      Constructor
    //-------------------------------------------------------------------------
    public:
        User(string username);


    //-------------------------------------------------------------------------
    //      Methods
    //-------------------------------------------------------------------------
    public:
        void follow(string username);
        void send(string message);
        string getUsername();
    };
}
