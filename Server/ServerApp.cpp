//
// Created by Farias, Karine on 3/17/21.
//
#include "include/ServerCommunicationManager.h"
#include "include/ReplicManager.hpp"
#include "../Utils/Types.h"
#include "../Utils/StringUtils.h"
#include "../Utils/Logger.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace socialine::utils;

///Function to check the parameters and check the execution mode to define log level
///@param: argc - count of parameters passed to the function
///@param: argv - vector containing the arguments passed to the function
///@returns: log level code
///Log Level ERROR: Messages reporting unrecoverable errors
///Log Level INFO:  Messages reporting operations status
///Log Level DEBUG: Messages containing data (variable contents, returning values) for debug purposes
///Log Level ALL:   All messages explained above will be shown

int check_mode(int argc, char* argv[]);

///Main function to orchestrate app execution
int main(int argc, char* argv[])
{
    ReplicManager::run();

    exit(0); // TEMP - REPLIC MANAGER TESTS

    Logger.set_log_level(check_mode(argc, argv));

    ServerCommunicationManager server;

    server.listenForBroadcast();
    server.start();

    //ToDo: call function to persist server data

    Logger.write_debug("Closing Server Session");

    return(0);
}

int check_mode(int argc, char* argv[])
{
    int level = LEVEL_ALL;

    if(argc > 1) {
        std::string argument = StringUtils::to_upper(argv[1]);

        if(!argument.compare("DEBUG"))
            level = LEVEL_DEBUG;
        else if(!argument.compare("ERROR"))
            level = LEVEL_ERROR;
        else if(!argument.compare("INFO"))
            level = LEVEL_INFO;
    }

    return level;
}
