//
// Created by Farias, Karine on 3/17/21.
//
#include "include/ServerCommunicationManager.h"
#include "../Utils/Types.h"
#include "../Utils/StringUtils.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

bool is_debug_mode(int argc, char* argv[]);


int main(int argc, char* argv[])
{
//    if (is_debug_mode(argc, argv))
//        Consolex::set_logger_level(LogLevel::DEBUG());
//    else
//        Consolex::set_logger_level(LogLevel::INFO());
    
    ServerCommunicationManager server;

    server.start();

    //ToDo: call function to persist server data

    cout << "Finishing session" << "\n";

//    Consolex::write_debug("I am finishing the session!");

    return(0);
}

bool is_debug_mode(int argc, char* argv[])
{
    return (argc > 1)
            && StringUtils::to_upper(std::string (argv[1])) == "TRUE";
}
