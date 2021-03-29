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

std::string trim(const std::string& str);

unordered_map<string, vector<string>> FileManager::read_profiles_file( )
{
    unordered_map<string, vector<string>> followers_map;
    fstream profiles_file("profiles.txt");
    string profiles_line;
    string username;
    vector<string> followers;
    int pos;

    if(!profiles_file)
    {
        cout << "Unable to open file" << NEW_LINE;
    }
    else
    {
        while(getline(profiles_file, profiles_line))
        {
            profiles_line = trim( profiles_line );
            pos = profiles_line.find_first_of(':', 0);
            username = profiles_line.substr(0, pos);
            profiles_line.erase(0, pos+2);
            cout << username << " should follow: " << profiles_line << NEW_LINE;
            pos = profiles_line.find_first_of(' ', 0);

            while(pos > 0)
            {
                followers.push_back(profiles_line.substr(0, pos));
                cout << "User: " << username << " Following: " << profiles_line.substr(0, pos) << NEW_LINE;
                profiles_line.erase(0, pos+1);
                pos = profiles_line.find_first_of(' ', 0);
            }

            followers.push_back(profiles_line);
            cout << "User: " << username << " Following: " << profiles_line.substr(0, pos) << NEW_LINE;
            followers_map[username] = followers;
        }
    }

    profiles_file.close();

    return followers_map;
}

void FileManager::write_profiles_file( std::unordered_map<std::string, std::vector<std::string>> final_followers_map )
{

}

std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');

    return str.substr(first, (last - first + 1));
}