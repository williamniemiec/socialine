#include <iostream>
#include <cstdio>
#include <string>
#include "LogMessage.hpp"

using namespace wniemiec::io::consolex;

//-------------------------------------------------------------------------
//		Attributes
//-------------------------------------------------------------------------
const int LogMessage::PADDING_LEFT = 8;


//-------------------------------------------------------------------------
//		Constructor
//-------------------------------------------------------------------------
LogMessage::LogMessage()
{
    lastMessage = "";
}


//-------------------------------------------------------------------------
//		Methods
//-------------------------------------------------------------------------
void LogMessage::log(std::string type, std::string message, bool breakline)
{
    std::string format = "%-" + std::to_string(PADDING_LEFT) + "s %s";
    
    printf(format.c_str(), format_type(type).c_str(), message.c_str());

    if (breakline)
        std::cout << std::endl;

    lastMessage = message;
    lastMessageType = type;
}

void LogMessage::log(std::string type, std::string message)
{
    log(type, message, true);
}

std::string LogMessage::format_type(std::string type)
{
    return "[" + type + "]";
}

void LogMessage::clear()
{
    lastMessage = "";
    lastMessageType = "";
}


//-------------------------------------------------------------------------
//		Getters
//-------------------------------------------------------------------------
std::string LogMessage::get_last_message()
{
    return lastMessage;
}

std::string LogMessage::get_last_message_type()
{
    return lastMessageType;
}
