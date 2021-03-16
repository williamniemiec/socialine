#pragma once
#include <iostream>
#include <list>

using namespace std;

namespace view
{
    class HomeView
    {
    //-------------------------------------------------------------------------
    //      Attributes
    //-------------------------------------------------------------------------
    private:
        string username;


    //-------------------------------------------------------------------------
    //      Constructor
    //-------------------------------------------------------------------------
    public:
        HomeView(string username);


    //-------------------------------------------------------------------------
    //      Methods
    //-------------------------------------------------------------------------
    public:
        void render();

    private:
        void displayUserInfo();
        void printDiv();
        void printEndLine(int amount);
        void displayNotifications();
        list<string> generateNotifications();
        void displayCommandArea();
    };
}
