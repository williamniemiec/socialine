#pragma once
#include <iostream>
#include "../../../Utils/Types.h"

namespace models
{
    class Notification
    {
    //-------------------------------------------------------------------------
    //      Attributes
    //-------------------------------------------------------------------------
    private:
        std::string username;
        std::string message;
        std::string date;


    //-------------------------------------------------------------------------
    //      Constructor
    //-------------------------------------------------------------------------
    public:
        Notification(notification n);
        Notification(std::string username, uint32_t timestamp, std::string message);


    //-------------------------------------------------------------------------
    //      Methods
    //-------------------------------------------------------------------------
    public:
        std::string to_string();
    private:
        std::string getDateFromNotification(uint32_t timestamp);


    //-------------------------------------------------------------------------
    //      Getters
    //-------------------------------------------------------------------------
    public:
        std::string getUsername();
        std::string getMessage();
        std::string getDate();
    };
}
