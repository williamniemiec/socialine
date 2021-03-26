//
// Created by Farias, Karine on 3/17/21.
//

#ifndef SOCIALINEV2_PROFILESESSIONMANAGER_H
#define SOCIALINEV2_PROFILESESSIONMANAGER_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

class ProfileSessionManager {

    unordered_map<string, vector<string>> sessions_map;
    unordered_map<string, vector<string>> followers_map;

public:
    ProfileSessionManager()
    {
        //MOCK DOS DADOS SESSÃO
        vector<string> sessionid;
        vector<string> sessionid2;

        sessionid.push_back("012345");

        sessionid2.push_back("ABCDEF");
        sessionid2.push_back("GHIJKL");

        sessions_map["@fariask"] = sessionid;
        sessions_map["@thiago"] = sessionid2;

        //MOCK DE DADOS SEGUIDORES
        vector<string> followers;
        vector<string> followers2;

        followers.push_back("@thiago");
        followers.push_back("@victor");
        followers.push_back("@william");

        followers2.push_back("@fariask");
        followers2.push_back("@victor");
        followers2.push_back("@william");

        followers_map["@fariask"] = followers;
        followers_map["@thiago"] = followers2;
    }

    int login( std::string username, std::string session_id );
    int logout( std::string username, std::string session_id );
    int follow( std::string follower, std::string followed );

private:
    int create_user(std::string username, std::string session_id);
    int handle_session(std::string username, std::string session_id);

};


#endif //SOCIALINEV2_PROFILESESSIONMANAGER_H
