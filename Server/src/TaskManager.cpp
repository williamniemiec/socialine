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
    char* line = nullptr;
    vector<string> arguments;
    stringstream string_stream;

    string_stream << payload;

    while(!string_stream.eof())
    {
        string_stream.getline( line, MAX_DATA_SIZE );
        arguments.push_back( line );

        cout << line << NEW_LINE;
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