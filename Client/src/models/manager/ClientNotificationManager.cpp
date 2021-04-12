#include <cstring>
#include "../../../include/models/manager/ClientNotificationManager.h"
#include "../../../include/views/HomeView.hpp"

using namespace models::manager;

//-------------------------------------------------------------------------
//      Attributes
//-------------------------------------------------------------------------
std::list<models::IObserver*> ClientNotificationManager::observers;
std::list<std::string>* ClientNotificationManager::notifications;
ClientNotificationManager* ClientNotificationManager::instance;
views::HomeView* homeview;


//-------------------------------------------------------------------------
//      Constructor
//-------------------------------------------------------------------------
ClientNotificationManager::ClientNotificationManager()
{
}


//-------------------------------------------------------------------------
//      Methods
//-------------------------------------------------------------------------
void ClientNotificationManager::attach(models::IObserver* observer)
{
    observers.push_back(observer);
    std::cout << observers.size() << std::endl;

    homeview = dynamic_cast<views::HomeView*>(observer);
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

void ClientNotificationManager::receive_notification(std::string username, uint32_t timestamp, std::string message)
{
    std::list<std::string> bufferNotifications;

    bufferNotifications.push_back((new Notification(username, timestamp, message))->to_string());
    notifications = &bufferNotifications;

    notify_observers();
}

ClientNotificationManager* ClientNotificationManager::get_instance()
{
    if (instance == nullptr)
        instance = new ClientNotificationManager();

    return instance;
}