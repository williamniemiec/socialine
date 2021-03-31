#include <iostream>
#include "include/models/auth/IAuthenticator.hpp"
#include "include/models/auth/CLIAuthenticator.hpp"
#include "include/exceptions/InvalidCredentialsException.hpp"
#include "include/controllers/HomeController.hpp"

int main(int argc, char* argv[])
{
    try {
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
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}

