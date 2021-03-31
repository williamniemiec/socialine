#include <iostream>
#include "../../include/models/MessageHandler.hpp"
#include "../../../Utils/Types.h"

using namespace models;

//-------------------------------------------------------------------------
//      Attributes
//-------------------------------------------------------------------------
std::map<int, std::string> MessageHandler::messageMapping = create_map();


//-------------------------------------------------------------------------
//      Constructor
//-------------------------------------------------------------------------
MessageHandler::MessageHandler()
{
}


//-------------------------------------------------------------------------
//      Methods
//-------------------------------------------------------------------------
std::map<int, std::string> MessageHandler::create_map()
{
    std::map<int, std::string> mapping;

    mapping[ERROR_INVALID_COMMAND] = "Invalid command";
    mapping[ERROR_START_WITH_LOGIN] = "Start with login";
    mapping[ERROR_SESSION_ALREADY_STARTED] = "Session already started";
    mapping[ERROR_SERVER_DOESNT_EXIST] = "Server does not exist";
    mapping[ERROR_SESSIONS_LIMIT] = "Session limit reached";
    mapping[ERROR_FOLLOW_YOURSELF] = "You cannot follow yourself";
    mapping[ERROR_OTHERS] = "Unknown error";
    mapping[MESSAGE_SHUT_DOWN] = "Shutting down";

    return mapping;
}

std::string MessageHandler::fromCode(int code)
{
    if (code == 0)
        return "Success!";
        
    if (messageMapping.count(code) == 0)
        return "Unknown error";

    return messageMapping[code];
}
