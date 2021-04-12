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
    date = get_date_from_notification(n.timestamp);
}

Notification::Notification(std::string username, uint32_t timestamp, std::string message)
{
    this->username = username;
    this->message = message;
    date = get_date_from_notification(timestamp);
}


//-------------------------------------------------------------------------
//      Methods
//-------------------------------------------------------------------------
std::string Notification::get_date_from_notification(uint32_t rawtime)
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
    return  "[ " + get_username() + " ]"
            + "  { " + get_date() + " }"
            + " " + get_message();
}


//-------------------------------------------------------------------------
//      Getters
//-------------------------------------------------------------------------
std::string Notification::get_username()
{
    return username;
}

std::string Notification::get_message()
{
    return message;
}

std::string Notification::get_date()
{
    return date;
}
