//
// Created by Farias, Karine on 3/25/21.
//

#include "../include/TaskManager.h"
#include "../include/ProfileSessionManager.h"
#include "../include/ServerNotificationManager.h"
#include "../include/ServerCommunicationManager.h"
#include "../../Utils/Types.h"
#include "../../Utils/StringUtils.hpp"
#include "../../Utils/wniemiec/io/consolex/Consolex.hpp"

using namespace wniemiec::io::consolex;
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
            Consolex::write_debug("Hi! This is the task Manager running command LOGIN");
            code = ProfileSessionManager::profileSessionManager.login(arguments[1], session_id);
            notifications = notification_manager.read_notifications(arguments[1]);
            for(int i = 0; i < notifications.size(); i++ )
            {
                comunication_manager.sendNotification( session_id, notifications[i] );
            }
            break;
        case CMD_FOLLOW:
            Consolex::write_debug("Hi! This is the task Manager running command FOLLOW");
            Consolex::write_debug("Follower: " + arguments[1] + " Followed: " + arguments[2]);
            code = ProfileSessionManager::profileSessionManager.follow(arguments[1], arguments[2]);
            break;
        case CMD_LOGOUT:
            Consolex::write_debug("Hi! This is the task Manager running command LOGOUT");
            Consolex::write_debug("session id: " + session_id);
            ProfileSessionManager::profileSessionManager.logout(arguments[1], session_id);
            break;
        case CMD_TWEET:
            Consolex::write_debug("Hi! This is the task Manager running command TWEET");
            notifications_map = notification_manager.tweet( arguments[1], arguments[2], arguments[0] );
            for (auto const& x : notifications_map)
            {
                comunication_manager.sendNotification( x.first, x.second );
            }
            break;
        default:
            Consolex::write_debug("Oops! Something went wrong!");
    }

    return code;
}