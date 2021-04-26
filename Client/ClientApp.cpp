#include <iostream>

#include "include/models/auth/IAuthenticator.hpp"
#include "include/models/auth/CLIAuthenticator.hpp"
#include "include/exceptions/InvalidCredentialsException.hpp"
#include "include/controllers/HomeController.hpp"
#include "include/controllers/HomeCLIController.hpp"

#include "../Utils/StringUtils.h"
#include "../Utils/Logger.h"

using namespace socialine::utils;

//-------------------------------------------------------------------------
//      Prototypes
//-------------------------------------------------------------------------
void run_gui(int argc, char* argv[]);
void run_cli(int argc, char* argv[]);
bool is_debug_mode(int argc, char* argv[]);


//-------------------------------------------------------------------------
//      Main
//-------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    //ToDo: better way to validate inputs
    if(argc < 5)
        Logger.write_error("Please, inform all parameters in the following order: user, server, port, debug");
        return -1;

    try {
        if (is_debug_mode(argc, argv))
        {
            Logger.set_log_level(LEVEL_DEBUG);
            run_cli(argc, argv);
        }
        else
        {
            run_gui(argc, argv);
        }
    }
    catch (std::exception &e) {
        Logger.write_error(e.what());
        return -1;
    }

    return 0;
}


//-------------------------------------------------------------------------
//      Methods
//-------------------------------------------------------------------------
bool is_debug_mode(int argc, char* argv[])
{
    return (argc > 4)
            && StringUtils::to_upper(std::string (argv[4])) == "TRUE";
}

void run_gui(int argc, char* argv[])
{
    // Home must be created before 'establish_connection'
    controllers::HomeController* home = new controllers::HomeController(argv[1]);

    models::auth::IAuthenticator* auth = new models::auth::CLIAuthenticator(argc, argv);
    auth->login();

    wxApp::SetInstance(home);
    wxEntryStart(argc, argv);
    wxTheApp->CallOnInit();
    wxTheApp->OnRun();
    wxTheApp->OnExit();
    wxEntryCleanup();
}

void run_cli(int argc, char* argv[])
{
    controllers::HomeCLIController* home = new controllers::HomeCLIController(argv[1]);

    models::auth::IAuthenticator* auth = new models::auth::CLIAuthenticator(argc, argv);
    auth->login();

    home->open();
}
