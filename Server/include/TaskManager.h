//
// Created by Farias, Karine on 3/25/21.
//

#ifndef SOCIALINE_TASKMANAGER_H
#define SOCIALINE_TASKMANAGER_H

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

class TaskManager {

public:
    int run_command(int type, std::string payload, std::string session_id );
/*
private:
    std::vector<std::string> parse_command( std::string payload );*/

};


#endif //SOCIALINE_TASKMANAGER_H