#pragma once
#include <iostream>

using namespace std;

namespace client::models
{
    class NotificationManager
    {
    //-------------------------------------------------------------------------
    //      Attributes
    //-------------------------------------------------------------------------
    private:
        string profile;


    //-------------------------------------------------------------------------
    //      Constructor
    //-------------------------------------------------------------------------
    public:
        NotificationManager(string profile);


    //-------------------------------------------------------------------------
    //      Methods
    //-------------------------------------------------------------------------
    public:
        void send(string message, string followed);
    };
}
