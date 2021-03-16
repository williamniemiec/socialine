#include <iostream>
#include <list>
#include <cstdlib>
#include "client/views/HomeView.hpp"
#include "client/models/Notification.hpp"

using namespace std;
using namespace client::views;

//-------------------------------------------------------------------------
//      Constructor
//-------------------------------------------------------------------------
HomeView::HomeView(controllers::HomeController* homeController, string username)
{
    this->username = username;
    this->homeController = homeController;
}


//-------------------------------------------------------------------------
//      Methods
//-------------------------------------------------------------------------
void HomeView::render()
{
    while (true)
    {
        displayUserInfo();
        displayNotifications();
        displayCommandArea();
        system("clear");
    }
}

void HomeView::displayUserInfo()
{
    printDiv();
    cout << "Welcome back " << this->username << endl;
    printDiv();

    printEndLine(2);
}

void HomeView::printDiv()
{
    cout << "--------------------------------------------------------------------------------" << endl;
}

void HomeView::printEndLine(int amount)
{
    for (int i = 0; i < amount; i++)
    {
        cout << endl;
    }
}

void HomeView::displayNotifications()
{
    for (client::models::Notification* notification : homeController->getNotifications())
    {
        printDiv();
        writeNotificationOwner(notification->getUsername());
        writeNotificationMessage(notification->getMessage());
    }

    printDiv();
    printEndLine(3);
}

void HomeView::writeNotificationOwner(string owner)
{
    cout << "[" << owner << "]";
}

void HomeView::writeNotificationMessage(string message)
{
    cout << "  " << message << endl;
}

void HomeView::displayCommandArea()
{
    string command;

    cout << "Command (or Ctrl + C to exit): ";
    getline(cin, command);

    this->homeController->parseCommand(command);
}
