#include <iostream>
#include "client/Client.h"
#include "client/view/HomeView.h"

using namespace std;
using namespace client;

//-------------------------------------------------------------------------
//      Constructor
//-------------------------------------------------------------------------
Client::Client(string username, string server, string port)
{
    this->username = username;
    this->server = server;
    this->port = port;
};


//-------------------------------------------------------------------------
//      Methods
//-------------------------------------------------------------------------
void Client::render()
{
    view::HomeView* view = new view::HomeView(username);
    view->render();
}
