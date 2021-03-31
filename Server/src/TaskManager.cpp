//
// Created by Farias, Karine on 3/25/21.
//

#include "../include/TaskManager.h"
#include "../include/ProfileSessionManager.h"
#include "../include/ServerNotificationManager.h"
#include "../include/ServerCommunicationManager.h"
#include "../../Utils/Types.h"
#include "../../Utils/StringUtils.hpp"

using namespace std;

ServerNotificationManager notification_manager;
ServerCommunicationManager comunication_manager;

int TaskManager::run_command(int type, string payload, string session_id )
{
    int code = 0;

    vector<notification> notifications;
    std::unordered_map<std::string, notification> notifications_map;
    vector<string> arguments;
    arguments = utils::StringUtils::split(payload, "\n");


    switch(type)
    {
        case CMD_LOGIN:
            cout << "Hi! This is the task Manager running command LOGIN" << NEW_LINE;
            code = ProfileSessionManager::profileSessionManager.login(arguments[1], session_id);
            notifications = notification_manager.read_notifications(arguments[1]);
            for(int i = 0; i < notifications.size(); i++ )
            {
                comunication_manager.sendNotification( session_id, notifications[i] );
            }
            break;
        case CMD_FOLLOW:
            cout << "Hi! This is the task Manager running command FOLLOW" << NEW_LINE;
            cout << "Follower: " << arguments[1] << "Followed: " << arguments[2];
            code = ProfileSessionManager::profileSessionManager.follow(arguments[1], arguments[2]);
            break;
        case CMD_LOGOUT:
            cout << "Hi! This is the task Manager running command LOGOUT" << NEW_LINE;
            ProfileSessionManager::profileSessionManager.logout(arguments[1], session_id);
            break;
        case CMD_TWEET:
            cout << "Hi! This is the task Manager running command TWEET" << NEW_LINE;
            notifications_map = notification_manager.tweet( arguments[1], arguments[2], arguments[0] );
            for (auto const& x : notifications_map)
            {
                comunication_manager.sendNotification( x.first, x.second );
            }
            break;
        default:
            cout << "Oops! Something went wrong!" << NEW_LINE;
    }

    return code;
}