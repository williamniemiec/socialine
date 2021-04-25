//
// Created by Farias, Karine on 3/17/21.
//

#include "../include/ServerNotificationManager.h"

using namespace std;

ProfileSessionManager ProfileSessionManager::profileSessionManager;

std::unordered_map<std::string, std::vector<notification>> ServerNotificationManager::pending_notifications;
sem_t ServerNotificationManager::notifications_semaphore;


std::vector<notification> ServerNotificationManager::read_notifications( std::string username )
{
    vector<notification> user_notifications;

    sem_wait(&notifications_semaphore);

    user_notifications = pending_notifications[username];
    pending_notifications[username].clear( );

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

    followed_by = ProfileSessionManager::profileSessionManager.read_followed( username );

    for(int i = 0; i < followed_by.size( ); i++ )
    {
        followed = followed_by[i];
        open_sessions = ProfileSessionManager::profileSessionManager.read_open_sessions(followed);
        if(open_sessions.size( ) == 0)
        {
            add_pending_notification( followed, current_notification );
        }
        else
        {
            for(int j = 0; j < open_sessions.size( ); j++ )
            {
                notifications_to_send[open_sessions[j]] = current_notification;
            }
        }
    }

    return notifications_to_send;

}

void ServerNotificationManager::add_pending_notification( std::string followed, notification current_notification )
{
    sem_wait(&notifications_semaphore);

    pending_notifications[followed].push_back(current_notification);

    sem_post(&notifications_semaphore);
}