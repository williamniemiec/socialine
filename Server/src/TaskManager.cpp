//
// Created by Farias, Karine on 3/25/21.
//

#include "../include/TaskManager.h"
#include "../include/ProfileSessionManager.h"
#include "../include/ServerCommunicationManager.h"

using namespace std;

vector<string> TaskManager::parse_command( string payload )
{
    //Arguments in a payload are separated by new line (\n)
    //This method receives a payload and returns the arguments list
    vector<string> arguments;

    std::string::size_type first_pos;
    std::string::size_type last_pos;
    first_pos = 0;

    do {
        last_pos = payload.find('\n', first_pos);
        if (last_pos == std::string::npos)
            break;

        arguments.push_back(payload.substr(first_pos, last_pos));
        first_pos = last_pos+1;
    } while(last_pos != std::string::npos);

    // debug print
    for(string arg : arguments)
        cout << "[TaskManager] parse_command arg = " << arg << endl;

    return arguments;
}

int TaskManager::run_command(int type, string payload, string session_id )
{
    int code = 0;

    vector<string> arguments;
    arguments = parse_command( payload );

    switch(type)
    {
        case CMD_LOGIN:
            cout << "Hi! This is the task Manager running command LOGIN" << NEW_LINE;
            code = ProfileSessionManager::profileSessionManager.login(arguments[1], session_id);
            //ServerCommunicationManager::sendNotification( session_id, notification );
            break;
        case CMD_FOLLOW:
            cout << "Hi! This is the task Manager running command FOLLOW" << NEW_LINE;
            cout << "Follower: " << arguments[1] << "Followed: " << arguments[2];
            code = ProfileSessionManager::profileSessionManager.follow(arguments[1], arguments[2]);
            break;
        case CMD_LOGOUT:
            cout << "Hi! This is the task Manager running command LOGOUT" << NEW_LINE;
            code = ProfileSessionManager::profileSessionManager.logout(arguments[1], session_id);
            break;
        case CMD_TWEET:
            cout << "Hi! This is the task Manager running command TWEET" << NEW_LINE;
            //notificationManager.tweet( );
            break;
        default:
            cout << "Oops! Something went wrong!" << NEW_LINE;
    }

    return code;
}