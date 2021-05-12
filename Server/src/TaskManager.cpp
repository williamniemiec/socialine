//
// Created by Farias, Karine on 3/25/21.
//

#include "../../Utils/Logger.h"
#include "../include/TaskManager.h"
#include "../include/ProfileSessionManager.h"
#include "../include/ServerNotificationManager.h"
#include "../include/ServerCommunicationManager.h"

using namespace std;
using namespace socialine::utils;

//-------------------------------------------------------------------------
//		Attributes
//-------------------------------------------------------------------------
ServerNotificationManager notification_manager;
ServerCommunicationManager comunication_manager;


//-------------------------------------------------------------------------
//		Methods
//-------------------------------------------------------------------------
int TaskManager::run_command(int type, string payload, string session_id )
{
    int code = 0;
    vector<notification> notifications;
    std::unordered_map<std::string, notification> notifications_map;
    vector<string> arguments;
    arguments = StringUtils::split(payload, "\n");

    switch(type)
    {
        case CMD_LOGIN:
            code = ProfileSessionManager::profileSessionManager.login(arguments[1], session_id);
            notifications = notification_manager.read_notifications(arguments[1]);
            for(int i = 0; i < notifications.size(); i++ )
            {
                comunication_manager.sendNotification( session_id, notifications[i] );
            }
            break;
        case CMD_FOLLOW:
            code = ProfileSessionManager::profileSessionManager.follow(arguments[1], arguments[2]);
            break;
        case CMD_LOGOUT:
            ProfileSessionManager::profileSessionManager.logout(arguments[1], session_id);
            break;
        case CMD_TWEET:
            notifications_map = notification_manager.tweet( arguments[1], arguments[2], arguments[0] );
            for (auto const& x : notifications_map)
            {
                comunication_manager.sendNotification( x.first, x.second );
            }
            break;
        default:
            Logger.write_error("Oops! Something went wrong!");
    }

    return code;
}
