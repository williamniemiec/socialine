#ifndef SOCIALINEV2_PROFILESESSIONMANAGER_H
#define SOCIALINEV2_PROFILESESSIONMANAGER_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <semaphore.h>
#include <sstream>

#include "FileManager.h"
#include "../../Utils/IObservable.hpp"
#include "../../Utils/IObserver.hpp"
#include "../../Utils/Logger.h"
#include "../../Utils/Types.h"
#include "../../Utils/StringUtils.h"
#include "../include/ReplicManager.hpp"

using namespace socialine::utils;

/// <summary>
///     Responsible for managing client sessions.
/// <summary>
class ProfileSessionManager : public IObservable
{
//-------------------------------------------------------------------------
//		Attributes
//-------------------------------------------------------------------------
private:
    static std::unordered_map<std::string, std::vector<std::string>> sessions_map;
    static std::unordered_map<std::string, std::vector<std::string>> followers_map;
    static std::unordered_map<std::string, std::vector<std::string>> followed_by_map;
    sem_t write_session_semaphore;
    sem_t session_readers_mutex;
    int session_readers_count;
    sem_t write_followers_semaphore;
    sem_t follower_readers_mutex;
    int followers_readers_count;
    std::list<IObserver*> observers;
    std::string arg0;
    std::string arg1;
    std::string arg2;
    ReplicManager* rm;

public:
    static ProfileSessionManager profileSessionManager;
    FileManager myFileManager;


//-------------------------------------------------------------------------
//		Constructor
//-------------------------------------------------------------------------
public:
    ProfileSessionManager()
    {
        rm = ReplicManager::get_instance();
        observers = std::list<IObserver*>();
        arg0 = "";
        arg1 = "";
        arg2 = "";
        followers_map = myFileManager.read_profiles_file();
        observers.push_back(rm);

        sem_init(&write_session_semaphore, 1, 1);
        sem_init(&session_readers_mutex, 1, 1);
        session_readers_count = 0;

        sem_init(&write_followers_semaphore, 1, 1);
        sem_init(&follower_readers_mutex, 1, 1);
        followers_readers_count = 0;
    }


//-------------------------------------------------------------------------
//		Methods
//-------------------------------------------------------------------------
public:
    static std::unordered_map<std::string, std::vector<std::string>> get_sessions();
    static std::unordered_map<std::string, std::vector<std::string>> get_followers();
    static void add_session(std::string username, std::string session_id);
    static void add_follower(std::string follower, std::string followed);
    static void remove_session(std::string username);
    virtual void attach(IObserver* observer);
    virtual void detatch(IObserver* observer);
    virtual void notify_observers();
    int login( std::string username, std::string session_id );
    int logout( std::string username, std::string session_id );
    int follow( std::string follower, std::string followed );
    std::vector<std::string> read_followers(std::string username);
    std::vector<std::string> read_followed(std::string username);
    std::vector<std::string> read_open_sessions(std::string username);

private:
    int open_session(std::string username, std::string session_id);
    void close_session(std::string username, std::string session_id);
    int write_follower(std::string follower, std::string followed);
};

#endif //SOCIALINEV2_PROFILESESSIONMANAGER_H
