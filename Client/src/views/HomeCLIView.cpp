#include <iostream>
#include <vector>
#include <thread>
#include <list>
#include <cstdlib>
#include <sstream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "../../../Utils/StringUtils.hpp"
#include "../../include/views/HomeCLIView.hpp"
#include "../../include/services/ClientCommunicationManager.h"
#include "../../include/models/manager/ClientNotificationManager.h"
#include "../../include/models/manager/ClientConsoleManager.hpp"

using namespace views;
using namespace controllers;
using namespace models;

//-------------------------------------------------------------------------
//      Attributes
//-------------------------------------------------------------------------
const std::string HomeCLIView::TITLE = "SociaLine";


//-------------------------------------------------------------------------
//      Constructor & Destructors
//-------------------------------------------------------------------------
HomeCLIView::HomeCLIView(HomeCLIController *homeController, User *user)
{
    this->homeController = homeController;
    this->user = user;
}

HomeCLIView::~HomeCLIView()
{
}


//-------------------------------------------------------------------------
//      Methods
//-------------------------------------------------------------------------
void HomeCLIView::update(IObservable *observable, std::list<std::string> data)
{
    if (dynamic_cast<manager::ClientNotificationManager *>(observable) != nullptr)
    {
        std::cout << "--- Notifications ---" << std::endl;

        for (std::string message : data)
        {
            std::cout << message << std::endl;
        }

        std::cout << "\n--- END ---" << std::endl;
    }
    else if (dynamic_cast<manager::ClientConsoleManager *>(observable) != nullptr)
    {
        for (std::string message : data)
        {
            std::cout << message << std::endl;
        }
    }
}

void HomeCLIView::render()
{
    std::thread t(&HomeCLIView::run, this);

    t.join();
}

void HomeCLIView::run()
{
    std::string command;

    while (true)
    {
        std::getline(std::cin, command);

        this->homeController->do_command(command);
    }
}
