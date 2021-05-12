//
// Created by Farias, Karine on 4/25/21.
//

#include "Types.h"
#include "Logger.h"
#include "iostream"

using namespace socialine::utils;
using namespace std;

void SocialineLogger::set_log_level( int level )
{
    log_level = level;
}

void SocialineLogger::write_debug(std::string message)
{
    if(log_level == LEVEL_DEBUG or log_level == LEVEL_ALL)
        std::cout << "[DEBUG] " << message << std::endl;
}

void SocialineLogger::write_error(std::string message)
{
    if(log_level == LEVEL_ERROR or log_level == LEVEL_ALL)
        std::cout << "[ERROR] " << message << std::endl;
}

void SocialineLogger::write_info(std::string message)
{
    if(log_level == LEVEL_INFO or log_level == LEVEL_ALL)
        std::cout << "[INFO]  " << message << std::endl;
}

int SocialineLogger::get_log_level( )
{
    return log_level;
}