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
    this->username = username;
}

//-------------------------------------------------------------------------
//      Methods
//-------------------------------------------------------------------------
void HomeController::run()
{
    this->notificationManager = new models::NotificationManager(username);
    this->render();
}

void HomeController::render()
{
    views::HomeView* homeView = new views::HomeView(this, username);
    homeView->render();
}

void HomeController::parseCommand(string command)
{
    /*
        if (command is SEND)
            notificationManager->send(message)
        else if (command is FOLLOW)
            models::CommunicationManager::follow(username, followed)
    */
}
