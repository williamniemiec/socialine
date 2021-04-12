//
// Created by Farias, Karine on 3/18/21.
//

#include "../include/FileManager.h"
#include "../../Utils/Types.h"
#include "../../Utils/wniemiec/util/data/StringUtils.hpp"
#include "../../Utils/wniemiec/io/consolex/Consolex.hpp"

#include <iostream>
#include <fstream>

#include <unordered_map>
#include <string>
#include <vector>

using namespace wniemiec::io::consolex;
using namespace wniemiec::util::data;
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
        cout << "Unable to open file" << NEW_LINE;
    }
    else
    {
        while(getline(profiles_file, profiles_line))
        {
            profiles_line = StringUtils::trim(profiles_line);
            pos = profiles_line.find_first_of(':', 0);
            username = profiles_line.substr(0, pos);
            profiles_line.erase(0, pos+2);
            Consolex::write_debug(username + " should follow: " + profiles_line);
            pos = profiles_line.find_first_of(' ', 0);

            while(pos > 0)
            {
                followers.push_back(profiles_line.substr(0, pos));
                Consolex::write_debug("User: " + username + " Following: " + profiles_line.substr(0, pos));
                profiles_line.erase(0, pos+1);
                pos = profiles_line.find_first_of(' ', 0);
            }

            followers.push_back(profiles_line);
            Consolex::write_debug("User: " + username + " Following: " + profiles_line.substr(0, pos));
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
