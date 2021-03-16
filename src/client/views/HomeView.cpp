#include <iostream>
#include <list>
#include <cstdlib>
#include "client/view/HomeView.h"

using namespace std;
using namespace view;

//-------------------------------------------------------------------------
//      Constructor
//-------------------------------------------------------------------------
HomeView::HomeView(string username)
{
    this->username = username;
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
    list<string> notifications = generateNotifications();

    for (string notification : notifications)
    {
        printDiv();
        cout << notification << endl;
    }
    printDiv();

    printEndLine(3);
}

list<string> HomeView::generateNotifications()
{
    list<string> notifications;

    notifications.push_back("Notification 1");
    notifications.push_back("Notification 2");
    notifications.push_back("Notification 3");
    notifications.push_back("Notification 4");
    notifications.push_back("Notification 5");
    notifications.push_back("Notification 6");

    return notifications;
}

void HomeView::displayCommandArea()
{
    string command;

    cout << "Command (or Ctrl + C to exit): ";
    getline(cin, command);
    cout << endl << command << endl;
}
