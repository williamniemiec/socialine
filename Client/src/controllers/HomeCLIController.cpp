#include <csignal>
#include <cstdlib>
#include <pthread.h>
#include "../../include/controllers/HomeCLIController.hpp"
#include "../../include/models/command/CommandParser.hpp"
#include "../../include/models/manager/ClientNotificationManager.h"
#include "../../include/models/MessageHandler.hpp"
#include "../../include/views/HomeCLIView.hpp"
#include "../../include/exceptions/InvalidCredentialsException.hpp"
#include "../../../Utils/Types.h"

using namespace controllers;
using namespace models;
using namespace views;

//-------------------------------------------------------------------------
//      Attributes
//-------------------------------------------------------------------------
models::manager::ClientConsoleManager* HomeCLIController::consoleManager =
        new manager::ClientConsoleManager();
models::manager::ClientNotificationManager* HomeCLIController::notificationManager =
        models::manager::ClientNotificationManager::get_instance();
ClientCommunicationManager* HomeCLIController::communicationManager =
        new ClientCommunicationManager();
std::string HomeCLIController::command;
views::IView* HomeCLIController::homeView;


//-------------------------------------------------------------------------
//      Constructor & Destructor
//-------------------------------------------------------------------------
HomeCLIController::HomeCLIController(std::string username)
{
    this->user = new User(username);
    
    homeView = new HomeCLIView(this, user);
    notificationManager->attach(homeView);
    consoleManager->attach(homeView);

    on_shutdown();
}


//-------------------------------------------------------------------------
//      Methods
//-------------------------------------------------------------------------
void HomeCLIController::build_home_view()
{
    
    consoleManager->welcome_message(user->get_username());
    homeView->render();
}

void HomeCLIController::on_shutdown()
{
    signal(SIGINT, HomeCLIController::shutdown_handler);
}

void HomeCLIController::shutdown_handler()
{
    shutdown_handler(-1);
}

void HomeCLIController::shutdown_handler(int sigcode)
{
    communicationManager->logout();

    exit(0);
}

void HomeCLIController::open()
{
    build_home_view();
}

void HomeCLIController::do_command(std::string command)
{
    HomeCLIController::command = command;  // Avoids std::bad_aloc

    pthread_t commandThread;
    pthread_create(&commandThread, NULL, command_runner, static_cast<void*>(&HomeCLIController::command));
}

void* HomeCLIController::command_runner(void* command)
{
    std::string &strCommand = *(static_cast<std::string*>(command));

    print_message_code(parse_command(strCommand));

    return nullptr;
}

void HomeCLIController::print_message_code(int code)
{
    consoleManager->send(MessageHandler::from_code(code));
}

int HomeCLIController::parse_command(std::string command)
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

int HomeCLIController::parse_app_command(app_command appCommand)
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

