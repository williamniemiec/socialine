#pragma once
#include <iostream>
#include <list>
#include <wx/app.h>
#include "../../../Utils/Types.h"
#include "../views/IView.hpp"
#include "../services/ClientCommunicationManager.h"
#include "../models/manager/ClientNotificationManager.h"
#include "../models/manager/ClientConsoleManager.hpp"
#include "../models/User.hpp"
#include "../models/Notification.hpp"

namespace controllers
{
    class HomeController : public wxApp
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
        HomeController(std::string username);


    //-------------------------------------------------------------------------
    //      Methods
    //-------------------------------------------------------------------------
    public:
        virtual bool OnInit();
        void open();
        static void shutdown_handler(int sigcode);
        void shutdown_handler();
        void send_message(std::string message);
        void follow(std::string username);
    private:
        static int parse_command(std::string command);
        static void print_message_code(int code);
        void build_home_view();
        static int parse_app_command(app_command appCommand);
        void on_shutdown();
        static void* command_runner(void* command);
        void do_command(std::string command);
    };
}
