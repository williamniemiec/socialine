//
// Created by Farias, Karine on 3/18/21.
//

#ifndef PROJECT_FILEMANAGER_H
#define PROJECT_FILEMANAGER_H

#include <vector>
#include <unordered_map>

class FileManager {

public:
    std::string trim(const std::string& str);
    std::unordered_map<std::string, std::vector<std::string>> read_profiles_file( );
    void write_profiles_file( std::unordered_map<std::string, std::vector<std::string>> final_followers_map  );

};


#endif //PROJECT_FILEMANAGER_H
