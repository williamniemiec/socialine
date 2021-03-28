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
    //This method receives a payload and returns arguments list
    vector<string> arguments;
    int pos = 0, i = 0;

    cout << "I am parsing a commmand" << NEW_LINE;
    while(pos != -1)
    {
        pos = payload.find_first_of('\n', 0);
        arguments.push_back(payload.substr(0, pos));
        payload.erase(0, pos);
        cout << "This is the current argument:" << arguments[i] << NEW_LINE;
        i++;
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
            cout << "Hi, I am login in" << NEW_LINE;
            code = profile_and_session.login(arguments[0], session_id);
            break;
        case CMD_FOLLOW:
            cout << "Follow" << NEW_LINE;
            //string follower;
            //transformar payload em follower
            //code = profile_and_session.follow(username, follower);
            break;
        case CMD_LOGOUT:
            cout << "LOGOUT" << NEW_LINE;
            //code = profile_and_session.logout(username, session_id);
            break;
        default:
            cout << "Jesus, to no lugar errado!" << NEW_LINE;
    }

    return code;
}