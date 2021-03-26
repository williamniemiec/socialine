//
// Created by Farias, Karine on 3/18/21.
//

#include "../include/FileManager.h"
#include "../../Utils/Types.h"

#include <iostream>
#include <fstream>

#include <unordered_map>
#include <string>
#include <vector>

using namespace std;


void FileManager::read_profiles_file( )
{
    /*fstream profiles;

    profiles.open( "profiles.txt" , std::fstream::in );

    if(profiles.is_open())
    {
        cout << "I am profiles file. I am opened";
    }

    profiles.

    profiles.close();*/

    vector<string> followers;
    vector<string> followers2;

    followers.push_back("@thiago");
    followers.push_back("@victor");
    followers.push_back("@william");

    followers2.push_back("@fariask");
    followers2.push_back("@victor");
    followers2.push_back("@william");

    unordered_map<string, vector<string>> followers_map;

    followers_map["@fariask"] = followers;
    followers_map["@thiago"] = followers2;

    cout << "I am @fariask and I follow this accounts:" << NEW_LINE;
    for(int i = 0; i < 3; i++)
        cout << "\t" << followers_map["@fariask"][i] << NEW_LINE;

    cout << "I am @thiago and I follow this accounts:" << NEW_LINE;
    for(int j = 0; j < 3; j++)
        cout << "\t" << followers_map["@thiago"][j] << NEW_LINE;

}

void FileManager::write_notifications_file( )
{
    cout << "Hi! I am the File Manager" << NEW_LINE;
    cout << "I am writing notifications" << NEW_LINE;
}

void FileManager::write_profiles_file( )
{
    cout << "Hi! I am the File Manager" << NEW_LINE;
    cout << "I am writing a profile " << NEW_LINE;
}


void FileManager::read_notifications_file( )
{
    cout << "Hi! I am the File Manager" << NEW_LINE;
    cout << "I am reading notifications file" << NEW_LINE;

}