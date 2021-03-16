#pragma once
#include <iostream>
#include "client/models/NotificationManager.hpp"

using namespace std;

namespace client::controllers
{
    class HomeController
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
        HomeController(string username);


    //-------------------------------------------------------------------------
    //      Methods
    //-------------------------------------------------------------------------
    public:
        void run();
        void parseCommand(string command);

    private:
        void render();
    };
}
