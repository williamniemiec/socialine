//
// Created by Farias, Karine on 3/25/21.
//

#ifndef SOCIALINE_TASKMANAGER_H
#define SOCIALINE_TASKMANAGER_H

#include "../include/ProfileSessionManager.h"
#include "../include/ServerNotificationManager.h"
#include "../include/ServerCommunicationManager.h"
#include "../../Utils/Types.h"
#include "../../Utils/StringUtils.h"

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