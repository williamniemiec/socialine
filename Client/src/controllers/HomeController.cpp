#include <csignal>
#include <cstdlib>
#include <wx/image.h>
#include <pthread.h>
#include "../../include/wx_pch.h"
#include "../../include/controllers/HomeController.hpp"
#include "../../include/models/command/CommandParser.hpp"
#include "../../include/models/manager/ClientNotificationManager.h"
#include "../../include/models/MessageHandler.hpp"
#include "../../include/views/HomeView.hpp"
#include "../../include/exceptions/InvalidCredentialsException.hpp"
#include "../../../Utils/Types.h"

using namespace controllers;
using namespace models;
using namespace views;

//-------------------------------------------------------------------------
//      Attributes
//-------------------------------------------------------------------------
models::manager::ClientConsoleManager* HomeController::consoleManager =
        new manager::ClientConsoleManager();
models::manager::ClientNotificationManager* HomeController::notificationManager =
        new manager::ClientNotificationManager();
ClientCommunicationManager* HomeController::communicationManager =
        new ClientCommunicationManager();
std::string HomeController::command;


//-------------------------------------------------------------------------
//      Constructor & Destructor
//-------------------------------------------------------------------------
HomeController::HomeController(std::string username)
{
    this->user = new User(username);

    onShutdown();
}


//-------------------------------------------------------------------------
//      Methods
//-------------------------------------------------------------------------
bool HomeController::OnInit()
{
    bool wxsOK = true;

    wxInitAllImageHandlers();

    if (wxsOK)
    {
        buildHomeView();
    	homeView->Show();
    	SetTopWindow(homeView);
    }

    return wxsOK;
}

void HomeController::buildHomeView()
{
    homeView = new HomeView(this, user, 0);
    homeView->render();

    notificationManager->attach(homeView);
    //notificationManager->fetch_notifications();

    consoleManager->attach(homeView);
    consoleManager->welcomeMessage(user->getUsername());
}

void HomeController::onShutdown()
{
    signal(SIGINT, HomeController::shutdownHandler);
}

void HomeController::shutdownHandler()
{
    shutdownHandler(-1);
}

void HomeController::shutdownHandler(int sigcode)
{
    communicationManager->logout();

    exit(0);
}

void HomeController::open()
{
    OnInit();
}

void HomeController::sendMessage(std::string message)
{
    consoleManager->send("Sending message '" + message + "' ...");
    doCommand("SEND " + message);
}

void HomeController::doCommand(std::string command)
{
    HomeController::command = command;  // Avoids std::bad_aloc

    pthread_t commandThread;
    pthread_create(&commandThread, NULL, commandRunner, static_cast<void*>(&HomeController::command));
}

void* HomeController::commandRunner(void* command)
{
    std::string &strCommand = *(static_cast<std::string*>(command));

    printMessageCode(parseCommand(strCommand));

    return nullptr;
}

void HomeController::printMessageCode(int code)
{
    consoleManager->send(MessageHandler::fromCode(code));
}

int HomeController::parseCommand(std::string command)
{
    try
    {
        app_command appCommand = command::CommandParser::parse(command);

        return parseAppCommand(appCommand);
    }
    catch(std::invalid_argument &e)
    {
        consoleManager->send(e.what());

        return ERROR_INVALID_COMMAND;
    }
}

int HomeController::parseAppCommand(app_command appCommand)
{
    int returnCode = ERROR_INVALID_COMMAND;

    switch(appCommand.type)
    {
        case CMD_LOGIN:
            returnCode = ERROR_SESSION_ALREADY_STARTED;
            break;
        case CMD_FOLLOW:
            returnCode = communicationManager->follow(appCommand.arguments[0]);
            break;
        case CMD_TWEET:
            returnCode = communicationManager->tweet(appCommand.arguments[0]);
            break;
        case CMD_OTHERS:
            returnCode = ERROR_INVALID_COMMAND;
            break;
    }

    return returnCode;
}

void HomeController::follow(std::string username)
{
    consoleManager->send("Follow user " + username);
    doCommand("FOLLOW " + username);
}
