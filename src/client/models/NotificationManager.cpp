#include <iostream>
#include "client/models/NotificationManager.hpp"
#include "client/models/CommunicationManager.hpp"

using namespace std;
using namespace client::models;

//-------------------------------------------------------------------------
//      Constructor
//-------------------------------------------------------------------------
NotificationManager::NotificationManager(string profile)
{
    this->profile = profile;
}


//-------------------------------------------------------------------------
//      Methods
//-------------------------------------------------------------------------
void NotificationManager::send(string message)
{
    cout << "Sending message " << message << endl;

    models::CommunicationManager::sendNotification(this->profile, message);

    cout << "Sent" << endl;
}
