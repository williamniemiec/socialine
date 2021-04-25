//
// Created by Farias, Karine on 3/18/21.
//

#include "../include/FileManager.h"

using namespace std;

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
        //Logger.write_error("Error opening profiles file");
    }
    else
    {
        //Logger.write_info("Successfully opened profiles file");
        while(getline(profiles_file, profiles_line))
        {
            profiles_line = StringUtils::trim(profiles_line);
            pos = profiles_line.find_first_of(':', 0);
            username = profiles_line.substr(0, pos);
            profiles_line.erase(0, pos+2);
            //Logger.write_debug(username + " should follow: " + profiles_line);
            pos = profiles_line.find_first_of(' ', 0);

            while(pos > 0)
            {
                followers.push_back(profiles_line.substr(0, pos));
                //Logger.write_debug("User: " + username + " Following: " + profiles_line.substr(0, pos));
                profiles_line.erase(0, pos+1);
                pos = profiles_line.find_first_of(' ', 0);
            }

            followers.push_back(profiles_line);
            //Logger.write_debug("User: " + username + " Following: " + profiles_line.substr(0, pos));
            followers_map[username] = followers;
        }
    }

    profiles_file.close();

    return followers_map;
}

void FileManager::write_profiles_file( std::unordered_map<std::string, std::vector<std::string>> final_followers_map )
{
    //ToDo: create write file;
}
