#pragma once
#include <iostream>
#include <list>
#include "client/controllers/HomeController.hpp"

using namespace std;

namespace views
{
    class HomeView
    {
    //-------------------------------------------------------------------------
    //      Attributes
    //-------------------------------------------------------------------------
    private:
        string username;
        controllers::HomeController* homeController;


    //-------------------------------------------------------------------------
    //      Constructor
    //-------------------------------------------------------------------------
    public:
        HomeView(controllers::HomeController* homeController, string username);


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
