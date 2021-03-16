#include "client/controllers/HomeController.hpp"
#include "client/views/HomeView.hpp"

using namespace std;
using namespace controllers;

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
    this->render();
}

void HomeController::render()
{
    views::HomeView* homeView = new views::HomeView(this, username);
    homeView->render();
}
