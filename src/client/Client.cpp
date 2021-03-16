#include <iostream>
#include "client/Client.hpp"
#include "client/controllers/HomeController.hpp"
#include "client/models/CommunicationManager.hpp"

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
void Client::login(string username, string server, string port)
{
    bool success = client::CommunicationManager::login(username, server, port);

    if (success)
    {
        controllers::HomeController* homeController = new controllers::HomeController(username);
        homeController->run();
    }
    else
    {
        cout << "Login failed" << endl;
    }
}
