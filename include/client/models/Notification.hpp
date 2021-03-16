#pragma once
#include <iostream>

using namespace std;

namespace client::models
{
    class Notification
    {
    //-------------------------------------------------------------------------
    //      Attributes
    //-------------------------------------------------------------------------
    private:
        string username;
        string message;


    //-------------------------------------------------------------------------
    //      Constructor
    //-------------------------------------------------------------------------
    public:
        Notification(string username, string message);


    //-------------------------------------------------------------------------
    //      Getters
    //-------------------------------------------------------------------------
    public:
        string getUsername();
        string getMessage();
    };
}
