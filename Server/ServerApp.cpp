//
// Created by Farias, Karine on 3/17/21.
//
#include "include/ServerCommunicationManager.h"
#include "../Utils/Types.h"
#include "../Utils/wniemiec/io/consolex/Consolex.hpp"
#include "../Utils/wniemiec/util/data/StringUtils.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;
using namespace wniemiec::io::consolex;
using namespace wniemiec::util::data;

//-------------------------------------------------------------------------
//      Prototypes
//-------------------------------------------------------------------------
bool is_debug_mode(int argc, char* argv[]);


//-------------------------------------------------------------------------
//      Main
//-------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    if (is_debug_mode(argc, argv))
        Consolex::set_logger_level(LogLevel::DEBUG());
    else
        Consolex::set_logger_level(LogLevel::INFO());
    
    ServerCommunicationManager server;

    server.start();

    //ToDo: call function to persist server data

    Consolex::write_debug("I am finishing the session!");

    return(0);
}

//-------------------------------------------------------------------------
//      Methods
//-------------------------------------------------------------------------
bool is_debug_mode(int argc, char* argv[])
{
    return  (argc > 1) 
            && StringUtils::to_upper(std::string (argv[1])) == "TRUE";
}
