//
// Created by Farias, Karine on 3/17/21.
//

#include "../include/ProfileSessionManager.h"
#include "../../Utils/Types.h"

using namespace std;

int ProfileSessionManager::login( std::string username, std::string session_id)
{
    int return_code = 0;
    //ToDo: Remove prints
    cout << "Welcome to Profile and Session Manager" << NEW_LINE ;
    cout << "I am trying to log you in, " << username << NEW_LINE;

    return_code = open_session(username, session_id);

    cout << "Finishing method login, code: " << return_code << NEW_LINE;

    return return_code;
}

int ProfileSessionManager::follow( std::string follower, std::string followed )
{

    int return_code = 0;

    //ToDo: Remove prints
    cout << "Welcome to Profile and Session Manager" << NEW_LINE ;
    cout << "I am trying to follow this profile: " << followed << NEW_LINE;

    if(follower == followed)
        return ERROR_FOLLOW_YOURSELF;

    return_code = write_follower(follower, followed);

    //ToDo: Remove prints
    cout << "Finishing method follow, code: " << return_code << NEW_LINE;

    return return_code;
}

int ProfileSessionManager::logout( std::string username, std::string session_id )
{
    cout << "LOGOUT LENG\n" << session_id.length() << endl;
    close_session(username, session_id);
    cout << session_id.length() << endl;

    return 0;
}

int ProfileSessionManager::open_session(std::string username, std::string session_id)
{
    int code = 0;

    session_id = myFileManager.trim(session_id);

    sem_wait(&write_session_semaphore);

    cout << "Active sessions: " << NEW_LINE;
    for(int i = 0; i < sessions_map[username].size(); i++ )
    {
        cout << sessions_map[username][i] << " ";
    }
    cout << NEW_LINE;

    if(sessions_map[username].size() >= 2)
    {
        code = ERROR_SESSIONS_LIMIT;
    }
    else if(sessions_map[username].size() == 1)
    {
        if(sessions_map[username][0] == session_id )
            code = ERROR_SESSION_ALREADY_STARTED;
    }

    if( code == 0 )
        sessions_map[username].push_back(session_id);

    cout << "After execution: " << NEW_LINE;
    for(int i = 0; i < sessions_map[username].size(); i++ )
    {
        cout << sessions_map[username][i] << " ";
    }

    cout << NEW_LINE;
    sem_post(&write_session_semaphore);

    return code;
}

void ProfileSessionManager::close_session(std::string username, std::string session_id)
{
    cout << "LENG\n" << session_id.length() << endl;
    session_id = myFileManager.trim(session_id);
    cout << session_id.length() << endl;

    //ToDo: clean up prints
    sem_wait(&write_session_semaphore);
    cout << session_id << endl;
    cout << "Active sessions: ";
    for(int i = 0; i < sessions_map[username].size(); i++ )
    {
        cout << sessions_map[username][i] << " ";
        if(sessions_map[username][i] == session_id)
        {
            sessions_map[username].erase(sessions_map[username].begin()+i);
        }
    }
    cout << "After deletion: ";
    for(int i = 0; i < sessions_map[username].size(); i++ )
    {
        cout << sessions_map[username][i] << " ";
    }
    sem_post(&write_session_semaphore);
}

std::vector<std::string> ProfileSessionManager::read_open_sessions(std::string username)
{
    vector<string> sessions_list;

    sem_wait(&session_readers_mutex);
    session_readers_count++;
    if(session_readers_count == 1)
        sem_wait(&write_session_semaphore);
    sem_post(&session_readers_mutex);

    sessions_list = sessions_map[username];

    sem_wait(&session_readers_mutex);
    session_readers_count--;
    if(session_readers_count == 0)
        sem_post(&write_session_semaphore);
    sem_post(&session_readers_mutex);

    return sessions_list;
}

vector<string> ProfileSessionManager::read_followers(std::string username)
{
    vector<string> followers_list;

    sem_wait(&follower_readers_mutex);
    followers_readers_count++;
    if(followers_readers_count == 1)
        sem_wait(&write_followers_semaphore);
    sem_post(&follower_readers_mutex);

    followers_list = followers_map[username];

    sem_wait(&follower_readers_mutex);
    followers_readers_count--;
    if(followers_readers_count == 0)
        sem_post(&write_followers_semaphore);
    sem_post(&follower_readers_mutex);

    return followers_list;
}

vector<string> ProfileSessionManager::read_followed(std::string username)
{
    vector<string> followed_list;

    sem_wait(&follower_readers_mutex);
    followers_readers_count++;
    if(followers_readers_count == 1)
        sem_wait(&write_followers_semaphore);
    sem_post(&follower_readers_mutex);

    followed_list = followed_by_map[username];

    sem_wait(&follower_readers_mutex);
    followers_readers_count--;
    if(followers_readers_count == 0)
        sem_post(&write_followers_semaphore);
    sem_post(&follower_readers_mutex);

    return followed_list;
}

int ProfileSessionManager::write_follower(std::string follower, std::string followed)
{

    int code = 0;

    sem_wait(&write_followers_semaphore);
    //ToDo: Remove prints
    cout << "I am " << follower << " and I follow this accounts:" << NEW_LINE;
    for (int k = 0; k < followers_map[follower].size() ; ++k)
        cout << "\t" << followers_map[follower][k] << NEW_LINE;

    //To ensure we only follow someone once
    if(followers_map[follower].size() != 0)
    {
        for(int i = 0; i < followers_map[follower].size(); i++)
        {
            if(followers_map[follower][i] == followed)
                code = ERROR_ALREADY_FOLLOWING_ACCOUNT;
        }
    }

    if(code == 0)
    {
        cout << "I am here!!!!" << NEW_LINE;
        followers_map[follower].push_back(followed);
        followed_by_map[followed].push_back(follower);
    }


    //ToDo: Remove prints
    cout << "I am " << follower << " and I follow this accounts:" << NEW_LINE;
    for (int k = 0; k < followers_map[follower].size() ; ++k)
        cout << "\t" << followers_map[follower][k] << NEW_LINE;

    cout << "\n\n";

    sem_post(&write_followers_semaphore);

    return code;

}