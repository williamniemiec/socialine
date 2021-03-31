//
// Created by Farias, Karine on 3/17/21.
//

#ifndef SOCIALINEV2_PROFILESESSIONMANAGER_H
#define SOCIALINEV2_PROFILESESSIONMANAGER_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <semaphore.h>
#include "FileManager.h"

class ProfileSessionManager {

    std::unordered_map<std::string, std::vector<std::string>> sessions_map;
    std::unordered_map<std::string, std::vector<std::string>> followers_map;
    std::unordered_map<std::string, std::vector<std::string>> followed_by_map;

    sem_t write_session_semaphore;
    sem_t session_readers_mutex;
    int session_readers_count;

    sem_t write_followers_semaphore;
    sem_t follower_readers_mutex;
    int followers_readers_count;

public:
    ProfileSessionManager()
    {
        FileManager myFileManager;

        followers_map = myFileManager.read_profiles_file();

        //ToDo: Remove cout
        std::cout << "Followers map size is: " << followers_map.size() << '\n';

        sem_init(&write_session_semaphore, 1, 1);
        sem_init(&session_readers_mutex, 1, 1);
        session_readers_count = 0;

        sem_init(&write_followers_semaphore, 1, 1);
        sem_init(&follower_readers_mutex, 1, 1);
        followers_readers_count = 0;

    }

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
