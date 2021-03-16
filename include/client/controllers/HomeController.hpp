#pragma once
#include <iostream>
#include <list>
#include "client/models/User.hpp"
#include "client/models/Notification.hpp"

using namespace std;
using namespace client::models;

namespace client::controllers
{
    class HomeController
    {
    //-------------------------------------------------------------------------
    //      Attributes
    //-------------------------------------------------------------------------
    private:
        models::User* user;


    //-------------------------------------------------------------------------
    //      Constructor
    //-------------------------------------------------------------------------
    public:
        HomeController(string username);


    //-------------------------------------------------------------------------
    //      Methods
    //-------------------------------------------------------------------------
    public:
        void run();
        void parseCommand(string command);
        list<models::Notification*> getNotifications();

    private:
        void render();
        list<models::Notification*> generateNotifications();
    };
}
