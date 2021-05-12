//
// Created by Farias, Karine on 3/17/21.
//
#include <thread>
#include "../include/ProfileSessionManager.h"


using namespace std;
using namespace socialine::utils;

std::unordered_map<std::string, std::vector<std::string>> ProfileSessionManager::sessions_map;
std::unordered_map<std::string, std::vector<std::string>> ProfileSessionManager::followers_map;
std::unordered_map<std::string, std::vector<std::string>> ProfileSessionManager::followed_by_map;

void ProfileSessionManager::attach(IObserver* observer)
{
    observers.push_back(observer);
}

void ProfileSessionManager::detatch(IObserver* observer)
{
    observers.remove(observer);
}

void ProfileSessionManager::notify_observers()
{
    std::list<std::string> body;
    body.push_back(arg0);
    body.push_back(arg1);
    body.push_back(arg2);
    
    for (IObserver* observer : observers)
    {
        std::thread([=]()
        {
            observer->update(this, body);
        }).detach();
    }
}

int ProfileSessionManager::login( std::string username, std::string session_id)
{
    int return_code = 0;

    //Logger.write_debug("Welcome to Profile and Session Manager");
    //Logger.write_debug("I am trying to log you in, " + username);

    return_code = open_session(username, session_id);

    //Logger.write_debug("Finishing method login, code: " + std::to_string(return_code));

    return return_code;
}

int ProfileSessionManager::follow( std::string follower, std::string followed )
{

    int return_code = 0;

    //Logger.write_debug("Welcome to Profile and Session Manager");
    //Logger.write_debug("I am trying to follow this profile: " + followed);

    if(follower == followed)
        return ERROR_FOLLOW_YOURSELF;

    return_code = write_follower(follower, followed);

    //Logger.write_debug("Finishing method follow, code: " + std::to_string(return_code));

    return return_code;
}

int ProfileSessionManager::logout( std::string username, std::string session_id )
{
    //Logger.write_debug("LOGOUT LENG: " + std::to_string(session_id.length()));
    close_session(username, session_id);
    //Logger.write_debug(std::to_string(session_id.length()));

    return 0;
}

int ProfileSessionManager::open_session(std::string username, std::string session_id)
{
    int code = 0;

    session_id = StringUtils::trim(session_id);

    sem_wait(&write_session_semaphore);

    Logger.write_debug("Active sessions:");
    stringstream activeSessions;
    
    for(int i = 0; i < sessions_map[username].size(); i++ )
    {
        activeSessions << sessions_map[username][i];
        activeSessions << " ";
    }
    Logger.write_debug(activeSessions.str());

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
    {
        sessions_map[username].push_back(session_id);

        std::cout << "SESSION ID: " << session_id << "\n";

        arg0 = "NEW_SESSION";
        arg1 = username;
        arg2 = session_id;
        notify_observers();
    }

    Logger.write_debug("After execution: ");
    stringstream activeSessionsAfterExecution;

    for(int i = 0; i < sessions_map[username].size(); i++ )
    {
        activeSessionsAfterExecution << sessions_map[username][i];
        activeSessionsAfterExecution << " ";
    }

    Logger.write_debug(activeSessionsAfterExecution.str());

    sem_post(&write_session_semaphore);

    return code;
}

void ProfileSessionManager::close_session(std::string username, std::string session_id)
{
    //Logger.write_debug("LENG: " + std::to_string(session_id.length()));
    session_id = StringUtils::trim(session_id);
    //Logger.write_debug(std::to_string(session_id.length()));

    sem_wait(&write_session_semaphore);
    //Logger.write_debug(std::to_string(session_id.length()));

    //Logger.write_debug("Active sessions:");
    stringstream activeSessions;

    for(int i = 0; i < sessions_map[username].size(); i++ )
    {
        activeSessions << sessions_map[username][i];
        activeSessions << " ";
        if(sessions_map[username][i] == session_id)
        {
            sessions_map[username].erase(sessions_map[username].begin()+i);
        }
    }
    //Logger.write_debug(activeSessions.str());

    //Logger.write_debug("After deletion:");
    stringstream sessionsAfterDelete;
    for(int i = 0; i < sessions_map[username].size(); i++ )
    {
        sessionsAfterDelete << sessions_map[username][i];
        sessionsAfterDelete << " ";
    }
    //Logger.write_debug(sessionsAfterDelete.str());

    arg0 = "CLOSE_SESSION";
    arg1 = username;
    notify_observers();

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
/*
    std::cout << "I am " + follower + " and I follow this accounts:\n";
    for (int k = 0; k < followers_map[follower].size() ; ++k)
        std::cout << "\t" + followers_map[follower][k] << std::endl;
  */  
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
        //Logger.write_debug("I am here!!!!");
        followers_map[follower].push_back(followed);
        followed_by_map[followed].push_back(follower);

        arg0 = "FOLLOW";
        arg1 = follower;
        arg2 = followed;
        notify_observers();
    }
/*
    std::cout << "I am " + follower + " and I follow this accounts:\n";
    for (int k = 0; k < followers_map[follower].size() ; ++k)
        std::cout << "\t" + followers_map[follower][k] << std::endl;
*/
    /*std::cout << "SOU " << followed << " SEGUIDO POR: \n";
    for (int k = 0; k < followed_by_map[followed].size() ; ++k)
        std::cout << "\t" + followed_by_map[followed][k] << std::endl;
    */
    sem_post(&write_followers_semaphore);

    return code;

}

std::unordered_map<std::string, std::vector<std::string>> ProfileSessionManager::get_sessions()
{
    return sessions_map;
}

std::unordered_map<std::string, std::vector<std::string>> ProfileSessionManager::get_followers()
{
    return followers_map;
}

void ProfileSessionManager::add_session(std::string username, std::string session_id)
{
    if (sessions_map.find(username) == sessions_map.end())
    {
        std::vector<std::string> sessions;
        
        sessions.push_back(session_id);
        sessions_map[username] = sessions;
    }
    else
    {
        sessions_map[username].push_back(session_id);
    }

    //std::cout << "Active sessions:\n";
    //stringstream activeSessions;
    
    /*for(int i = 0; i < sessions_map[username].size(); i++ )
    {
        std::cout << sessions_map[username][i] << " ";

    }*/
}

void ProfileSessionManager::add_follower(std::string follower, std::string followed)
{
    /*std::cout << "I am " + follower + " and I follow this accounts:\n";
    for (int k = 0; k < followers_map[follower].size() ; ++k)
        std::cout << "\t" + followers_map[follower][k] << std::endl;
*/
    //To ensure we only follow someone once
    if(followers_map[follower].size() != 0)
    {
        for(int i = 0; i < followers_map[follower].size(); i++)
        {
            if(followers_map[follower][i] == followed)
                return;
        }
    }

    followers_map[follower].push_back(followed);
    followed_by_map[followed].push_back(follower);
/*
    std::cout << "I am " + follower + " and I follow this accounts:\n";
    for (int k = 0; k < followers_map[follower].size() ; ++k)
        std::cout << "\t" + followers_map[follower][k] << std::endl;

    std::cout << "SOU " << followed << " SEGUIDO POR: \n";
    for (int k = 0; k < followed_by_map[followed].size() ; ++k)
        std::cout << "\t" + followed_by_map[followed][k] << std::endl;*/
}

void ProfileSessionManager::remove_session(std::string username)
{
    sessions_map.erase(username);
}
