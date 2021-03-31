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
        static void shutdownHandler(int sigcode);
        void shutdownHandler();
        void sendMessage(std::string message);
        void follow(std::string username);
    private:
        static int parseCommand(std::string command);
        static void printMessageCode(int code);
        void buildHomeView();
        static int parseAppCommand(app_command appCommand);
        void onShutdown();
        static void* commandRunner(void* command);
        void doCommand(std::string command);
    };
}
