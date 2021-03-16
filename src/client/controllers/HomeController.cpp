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
    this->render();
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
