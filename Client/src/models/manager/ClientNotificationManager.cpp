#include <cstring>
#include "../../../include/models/manager/ClientNotificationManager.h"

using namespace models::manager;

//-------------------------------------------------------------------------
//      Attributes
//-------------------------------------------------------------------------
std::list<models::IObserver*> ClientNotificationManager::observers;


//-------------------------------------------------------------------------
//      Methods
//-------------------------------------------------------------------------
void ClientNotificationManager::attach(models::IObserver* observer)
{
    observers.push_back(observer);
}

void ClientNotificationManager::detatch(models::IObserver* observer)
{
    observers.remove(observer);
}

void ClientNotificationManager::notify_observers()
{
    for (models::IObserver* observer : observers)
    {
        observer->update(this, *notifications);
    }
}

/*
void ClientNotificationManager::fetch_notifications()
{
    std::list<std::string> bufferNotifications;

    for (notification n : communicationManager->getNotifications())
    {
        bufferNotifications.push_back((new Notification(n))->to_string());
    }

    notifications = &bufferNotifications;

    notify_observers();
}
*/

void ClientNotificationManager::receive_notification(std::string username, uint32_t timestamp, std::string message)
{
    std::list<std::string> bufferNotifications;

    bufferNotifications.push_back((new Notification(username, timestamp, message))->to_string());

    notifications = &bufferNotifications;

    notify_observers();
}
