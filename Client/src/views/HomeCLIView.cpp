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
#include "../../include/views/HomeCLIView.hpp"
#include "../../include/services/ClientCommunicationManager.h"
#include "../../include/models/manager/ClientNotificationManager.h"
#include "../../include/models/manager/ClientConsoleManager.hpp"
#include "../../../Utils/wniemiec/io/consolex/Consolex.hpp"

using namespace views;
using namespace controllers;
using namespace models;
using namespace wniemiec::io::consolex;

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
            Consolex::write_line(message);
        }

        std::cout << "\n--- END ---" << std::endl;
    }
    else if (dynamic_cast<manager::ClientConsoleManager *>(observable) != nullptr)
    {
        for (std::string message : data)
        {
            Consolex::write_info(message);
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
    while (true)
    {
        std::string command = Consolex::read_line();

        this->homeController->do_command(command);
    }
}
