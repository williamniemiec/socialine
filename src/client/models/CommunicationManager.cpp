#include "client/models/CommunicationManager.hpp"

using namespace std;
using namespace client::models;

//-------------------------------------------------------------------------
//      Constructor
//-------------------------------------------------------------------------
CommunicationManager::CommunicationManager()
{
}


//-------------------------------------------------------------------------
//      Methods
//-------------------------------------------------------------------------
bool CommunicationManager::login(string username, string server, string port)
{
    return true;
}

void CommunicationManager::follow(string follower, string followed)
{

}

void CommunicationManager::sendNotification(string profile, string message)
{

}

list<client::models::Notification*> CommunicationManager::getNotifications()
{
    return list<client::models::Notification*>();
}
