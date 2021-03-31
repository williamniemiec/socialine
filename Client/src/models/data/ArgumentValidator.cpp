#include <iostream>
#include <regex>
#include "../../../include/models/data/ArgumentValidator.hpp"
#include "../../../../Utils/Types.h"
#include "../../../../Utils/StringUtils.hpp"

using namespace models::data;

//-------------------------------------------------------------------------
//      Constructor
//-------------------------------------------------------------------------
ArgumentValidator::ArgumentValidator()
{
}


//-------------------------------------------------------------------------
//      Methods
//-------------------------------------------------------------------------
bool ArgumentValidator::validate_username(std::string username)
{
    const std::regex REGEX_USERNAME("@[0-9A-z_]+");

    return std::regex_match(username, REGEX_USERNAME);
}

bool ArgumentValidator::validate_ip(std::string ip)
{
    const std::regex REGEX_IP("([0-9]{1,3}\\.){3}[0-9]{1,3}");

    return std::regex_match(ip, REGEX_IP);
}

bool ArgumentValidator::validate_port(std::string port)
{
    const std::regex REGEX_PORT("[0-9]{1,4}");

    return std::regex_match(port, REGEX_PORT);
}

bool ArgumentValidator::validate_command(std::string command)
{
    const std::regex REGEX_COMMAND("[A-Z]+[\\s\\t]+.+");

    return std::regex_match(utils::StringUtils::to_upper(command), REGEX_COMMAND);
}
