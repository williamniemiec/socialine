#pragma once
#include <iostream>
#include "client/models/Notification.hpp"

using namespace std;
using namespace client::models;

//-------------------------------------------------------------------------
//      Constructor
//-------------------------------------------------------------------------
Notification::Notification(string username, string message)
{
 this->username = username;
 this->message = message;
}


//-------------------------------------------------------------------------
//      Getters
//-------------------------------------------------------------------------
string Notification::getUsername()
{
    return this->username;
}

string Notification::getMessage()
{
    return this->message;
}
