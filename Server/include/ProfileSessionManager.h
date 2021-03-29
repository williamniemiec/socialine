//
// Created by Farias, Karine on 3/17/21.
//

#ifndef SOCIALINEV2_PROFILESESSIONMANAGER_H
#define SOCIALINEV2_PROFILESESSIONMANAGER_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include "FileManager.h"

using namespace std;

class ProfileSessionManager {

    unordered_map<string, vector<string>> sessions_map;
    unordered_map<string, vector<string>> followers_map;

public:
    ProfileSessionManager()
    {
        FileManager myFileManager;

        followers_map = myFileManager.read_profiles_file();
    }

    int login( std::string username, std::string session_id );
    int logout( std::string username, std::string session_id );
    int follow( std::string follower, std::string followed );

private:
    int create_user(std::string username, std::string session_id);
    int handle_session(std::string username, std::string session_id);

};


#endif //SOCIALINEV2_PROFILESESSIONMANAGER_H
