#pragma once
#include <iostream>
#include "client/models/User.hpp"

using namespace std;

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

    private:
        void render();
    };
}
