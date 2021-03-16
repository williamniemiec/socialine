#include "client/controllers/HomeController.hpp"
#include "client/views/HomeView.hpp"

using namespace std;
using namespace client::controllers;
using namespace client::views;
using namespace client::models;

//-------------------------------------------------------------------------
//      Constructor
//-------------------------------------------------------------------------
HomeController::HomeController(string username)
{
    this->user = new models::User(username);
}

//-------------------------------------------------------------------------
//      Methods
//-------------------------------------------------------------------------
void HomeController::run()
{
    render();
}

void HomeController::render()
{
    views::HomeView* homeView = new views::HomeView(this, user->getUsername());
    homeView->render();
}

void HomeController::parseCommand(string command)
{
    /*
        if (command is SEND)
            user->send(message)
        else if (command is FOLLOW)
            user->follow(followed)
    */
}

list<client::models::Notification*> HomeController::getNotifications()
{
    return generateNotifications();
}

list<client::models::Notification*> HomeController::generateNotifications()
{
    list<models::Notification*> notifications;

    models::Notification* n1 = new models::Notification("beltrano456", "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Dictum non consectetur a erat nam at.");
    models::Notification* n2 = new models::Notification("sicrano789", "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Dictum non consectetur a erat nam at.");

    notifications.push_back(n1);
    notifications.push_back(n2);

    return notifications;
}
