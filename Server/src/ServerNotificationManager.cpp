//
// Created by Farias, Karine on 3/17/21.
//
#include <thread>
#include "../../Utils/Logger.h"
#include "../include/ServerNotificationManager.h"
#include "../include/ProfileSessionManager.h"

using namespace std;

//-------------------------------------------------------------------------
//		Attributes
//-------------------------------------------------------------------------
ProfileSessionManager ProfileSessionManager::profileSessionManager;
std::unordered_map<std::string, std::vector<notification>> ServerNotificationManager::pending_notifications;
sem_t ServerNotificationManager::notifications_semaphore;


//-------------------------------------------------------------------------
//		Methods
//-------------------------------------------------------------------------
void ServerNotificationManager::attach(IObserver* observer)
{
    observers.push_back(observer);
}

void ServerNotificationManager::detatch(IObserver* observer)
{
    observers.remove(observer);
}

void ServerNotificationManager::notify_observers()
{
    std::list<std::string> body;
    body.push_back(arg0); 
    body.push_back(arg1); 
    body.push_back(arg2); 
    
    for (IObserver* observer : observers)
    {
        std::thread([=]()
        {
            observer->update(this, body);
        }).detach();
    }
}

std::vector<notification> ServerNotificationManager::read_notifications( std::string username )
{
    vector<notification> user_notifications;

    sem_wait(&notifications_semaphore);

    user_notifications = pending_notifications[username];
    pending_notifications[username].clear( );

    arg0 = "CLOSE";
    arg1 = username;
    notify_observers();

    sem_post(&notifications_semaphore);

    return user_notifications;
}

std::unordered_map<std::string, notification> ServerNotificationManager::tweet( std::string username, std::string message, std::string timestamp  )
{
    unordered_map<string, notification> notifications_to_send;
    vector<string> followed_by;
    vector<string> open_sessions;
    notification current_notification;
    string followed;

    current_notification.owner = username;
    //current_notification.timestamp = timestamp;
    current_notification._message = message;
    arg0 = "NEW";
    arg1 = username;
    arg2 = message;
    notify_observers();

    followed_by = ProfileSessionManager::profileSessionManager.read_followed( username );

    for(int i = 0; i < followed_by.size( ); i++ )
    {
        followed = followed_by[i];
        Logger.write_debug("Followed by: " + followed);
        open_sessions = ProfileSessionManager::profileSessionManager.read_open_sessions(followed);
        if(open_sessions.size( ) == 0)
        {
            add_pending_notification( followed, current_notification );
        }
        else
        {
            for(int j = 0; j < open_sessions.size( ); j++ )
            {
                Logger.write_debug("Open session: " + open_sessions[j]);
                notifications_to_send[open_sessions[j]] = current_notification;
            }
        }
    }

    return notifications_to_send;

}

std::unordered_map<std::string, std::vector<notification>> ServerNotificationManager::get_pending_notifications()
{
    return pending_notifications;
}

void ServerNotificationManager::add_pending_notification( std::string followed, notification current_notification )
{
    sem_wait(&notifications_semaphore);

    pending_notifications[followed].push_back(current_notification);

    sem_post(&notifications_semaphore);
}

void ServerNotificationManager::remove_pending_notification(std::string username)
{
    pending_notifications[username].clear();
}
