//
// Created by Farias, Karine on 3/25/21.
//

#include "../include/TaskManager.h"
#include "../include/ProfileSessionManager.h"
#include "../../Utils/Types.h"

using namespace std;

ProfileSessionManager profile_and_session;

vector<string> TaskManager::parse_command( string payload )
{
    //Arguments in a payload are separated by new line
    //This method receives a payload and returns the arguments list

    //ToDo: replace this by another parsing methods (that works :)
  
    vector<string> arguments;
    int pos = payload.find_first_of(NEW_LINE, 0);

    while(pos != 0 )
    {
        arguments.push_back(payload.substr(0, pos));
        payload.erase(0, pos);
        pos = payload.find_first_of(NEW_LINE, 0);
    }

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
            code = profile_and_session.login(arguments[0], session_id);
            break;
        case CMD_FOLLOW:
            cout << "Hi! This is the task Manager running command FOLLOW" << NEW_LINE;
            cout << "Follower: " << arguments[0] << "Followed: " << arguments[1];
            code = profile_and_session.follow(arguments[0], arguments[1]);
            break;
        case CMD_LOGOUT:
            cout << "Hi! This is the task Manager running command LOGOUT" << NEW_LINE;
            code = profile_and_session.logout(arguments[0], session_id);
            break;
        default:
            cout << "Oops! Something went wrong!" << NEW_LINE;
    }

    return code;
}