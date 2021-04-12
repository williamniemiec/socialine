#pragma once
#include <iostream>
#include <list>
#include "../../../Utils/Types.h"
#include "../views/IView.hpp"
#include "../services/ClientCommunicationManager.h"
#include "../models/manager/ClientNotificationManager.h"
#include "../models/manager/ClientConsoleManager.hpp"
#include "../models/User.hpp"
#include "../models/Notification.hpp"

namespace controllers
{
    class HomeCLIController
    {
    //-------------------------------------------------------------------------
    //      Attributes
    //-------------------------------------------------------------------------
    private:
        static views::IView* homeView;
        models::User* user;
        static ClientCommunicationManager* communicationManager;
        static models::manager::ClientNotificationManager* notificationManager;
        static models::manager::ClientConsoleManager* consoleManager;
        static std::string command;


    //-------------------------------------------------------------------------
    //      Constructor & Destructor
    //-------------------------------------------------------------------------
    public:
        HomeCLIController(std::string username);


    //-------------------------------------------------------------------------
    //      Methods
    //-------------------------------------------------------------------------
    public:
        void open();
        static void shutdown_handler(int sigcode);
        void shutdown_handler();
        void do_command(std::string command);
    private:
        static int parse_command(std::string command);
        static void print_message_code(int code);
        void build_home_view();
        static int parse_app_command(app_command appCommand);
        void on_shutdown();
        static void* command_runner(void* command);
    };
}
