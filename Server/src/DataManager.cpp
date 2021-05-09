#include "../include/DataManager.hpp"

//-------------------------------------------------------------------------
//		Attributes
//-------------------------------------------------------------------------
std::unordered_map<std::string, client_session> DataManager::sessions;
std::unordered_map<std::string, std::vector<std::string>> DataManager::followers;
std::unordered_map<std::string, std::vector<std::string>> DataManager::followedBy;
std::unordered_map<std::string, std::vector<notification>> DataManager::pendingNotifications;


//-------------------------------------------------------------------------
//		Methods
//-------------------------------------------------------------------------
void DataManager::new_session(std::string sessionId, client_session session)
{
    sessions[sessionId] = session;
}

void DataManager::remove_session(std::string sessionId)
{
    sessions.erase(sessionId);
}

bool DataManager::follow(std::string follower, std::string followed)
{
    bool success = true;
    
    if(followers[follower].size() != 0)
    {
        for(int i = 0; i < followers[follower].size(); i++)
        {
            if(followers[follower][i] == followed)
                success = false;
        }
    }

    if(success)
    {
        followers[follower].push_back(followed);
        followedBy[followed].push_back(follower);
    }

    return success;
}

void DataManager::new_pending_notification(std::string user, notification pendingNotification)
{
    if (pendingNotifications.find(user) == pendingNotifications.end())
    {
        std::vector<notification> notifications;
        notifications.push_back(pendingNotification);

        pendingNotifications.insert(std::make_pair(user, notifications));
    }
    else
    {
        std::vector<notification> notifications = pendingNotifications[user];
        notifications.push_back(pendingNotification);

        pendingNotifications.insert(std::make_pair(user, notifications));
    }
}

std::vector<notification> DataManager::read_pending_notification(std::string username)
{
    std::vector<notification> user_notifications;

    user_notifications = pendingNotifications[username];
    pendingNotifications[username].clear();

    return user_notifications;
}

std::unordered_map<std::string, client_session> DataManager::get_all_sessions()
{
    return sessions;
}

std::unordered_map<std::string, std::vector<std::string>> DataManager::get_all_followers()
{
    return followers;
}

std::unordered_map<std::string, std::vector<notification>> DataManager::get_all_pending_notifications()
{
    return pendingNotifications;
}

