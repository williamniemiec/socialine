#include <iostream>
#include <unistd.h>
#include <ctime>
#include <sstream>
#include "../../include/models/Notification.hpp"

using namespace models;

//-------------------------------------------------------------------------
//      Constructor
//-------------------------------------------------------------------------
Notification::Notification(notification n)
{
    username = n.owner;
    message = n._message;
    date = getDateFromNotification(n.timestamp);
}

Notification::Notification(std::string username, uint32_t timestamp, std::string message)
{
    this->username = username;
    this->message = message;
    date = getDateFromNotification(timestamp);
}


//-------------------------------------------------------------------------
//      Methods
//-------------------------------------------------------------------------
std::string Notification::getDateFromNotification(uint32_t rawtime)
{
    time_t t = rawtime;
    struct tm timeinfo = *localtime(&t);

    std::stringstream date;

    if ((timeinfo.tm_mon+1) < 10)
        date << "0";

    date << timeinfo.tm_mon+1;
    date << "/";

    if ((timeinfo.tm_mday+1) < 10)
        date << "0";

    date << timeinfo.tm_mday+1;
    date << "/";
    date << timeinfo.tm_year+1900;
    date << " ";

    if ((timeinfo.tm_hour) < 10)
        date << "0";

    date << timeinfo.tm_hour;
    date << ":";

    if ((timeinfo.tm_min) < 10)
        date << "0";

    date << timeinfo.tm_min;
    date << ":";

    if ((timeinfo.tm_sec) < 10)
        date << "0";

    date << timeinfo.tm_sec;

    return date.str();
}

std::string Notification::to_string()
{
    return  "[ " + getUsername() + " ]"
            + "  { " + getDate() + " }"
            + " " + getMessage();
}


//-------------------------------------------------------------------------
//      Getters
//-------------------------------------------------------------------------
std::string Notification::getUsername()
{
    return username;
}

std::string Notification::getMessage()
{
    return message;
}

std::string Notification::getDate()
{
    return date;
}
