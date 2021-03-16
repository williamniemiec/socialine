#include <iostream>
#include "client/models/User.hpp"
#include "client/models/CommunicationManager.hpp"
#include "client/models/NotificationManager.hpp"

using namespace std;
using namespace client::models;

//-------------------------------------------------------------------------
//      Constructor
//-------------------------------------------------------------------------
User::User(string username)
{
    this->username = username;
    this->notificationManager = new models::NotificationManager(username);
}

//-------------------------------------------------------------------------
//      Methods
//-------------------------------------------------------------------------
void User::follow(string username)
{
    models::CommunicationManager::follow(this->username, username);
}

void User::send(string message)
{
    this->notificationManager->send(message);
}

string User::getUsername()
{
    return this->username;
}
