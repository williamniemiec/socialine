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
        models::manager::ClientNotificationManager::get_instance();
ClientCommunicationManager* HomeController::communicationManager =
        new ClientCommunicationManager();
std::string HomeController::command;
views::IView* HomeController::homeView;


//-------------------------------------------------------------------------
//      Constructor & Destructor
//-------------------------------------------------------------------------
HomeController::HomeController(std::string username)
{
    this->user = new User(username);
    
    homeView = new HomeView(this, user, 0);
    notificationManager->attach(homeView);
    consoleManager->attach(homeView);

    on_shutdown();
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
        build_home_view();
    	homeView->Show();
    	SetTopWindow(homeView);
    }

    return wxsOK;
}

void HomeController::build_home_view()
{
    
    homeView->render();
    consoleManager->welcome_message(user->get_username());
}

void HomeController::on_shutdown()
{
    signal(SIGINT, HomeController::shutdown_handler);
}

void HomeController::shutdown_handler()
{
    shutdown_handler(-1);
}

void HomeController::shutdown_handler(int sigcode)
{
    communicationManager->logout();

    exit(0);
}

void HomeController::open()
{
    OnInit();
}

void HomeController::send_message(std::string message)
{
    consoleManager->send("Sending message '" + message + "' ...");
    do_command("SEND " + message);
}

void HomeController::do_command(std::string command)
{
    HomeController::command = command;  // Avoids std::bad_aloc

    pthread_t commandThread;
    pthread_create(&commandThread, NULL, command_runner, static_cast<void*>(&HomeController::command));
}

void* HomeController::command_runner(void* command)
{
    std::string &strCommand = *(static_cast<std::string*>(command));

    print_message_code(parse_command(strCommand));

    return nullptr;
}

void HomeController::print_message_code(int code)
{
    consoleManager->send(MessageHandler::from_code(code));
}

int HomeController::parse_command(std::string command)
{
    try
    {
        app_command appCommand = command::CommandParser::parse(command);

        return parse_app_command(appCommand);
    }
    catch(std::invalid_argument &e)
    {
        consoleManager->send(e.what());

        return ERROR_INVALID_COMMAND;
    }
}

int HomeController::parse_app_command(app_command appCommand)
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
    do_command("FOLLOW " + username);
}
