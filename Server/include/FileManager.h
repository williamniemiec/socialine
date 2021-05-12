//
// Created by Farias, Karine on 3/18/21.
//

#ifndef PROJECT_FILEMANAGER_H
#define PROJECT_FILEMANAGER_H

#include "../../Utils/Logger.h"
#include "../../Utils/StringUtils.h"

#include <iostream>
#include <fstream>

#include <unordered_map>
#include <string>
#include <vector>

using namespace socialine::utils;

/// <summary>
///     Responsible for managing files.
/// <summary>
class FileManager 
{
//-------------------------------------------------------------------------
//		Methods
//-------------------------------------------------------------------------
public:
    std::unordered_map<std::string, std::vector<std::string>> read_profiles_file( );
    void write_profiles_file( std::unordered_map<std::string, std::vector<std::string>> final_followers_map  );
};


#endif //PROJECT_FILEMANAGER_H
