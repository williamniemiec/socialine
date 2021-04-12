#include "../../../include/models/auth/CLIAuthenticator.hpp"
#include "../../../include/models/command/CommandParser.hpp"
#include "../../../include/exceptions/InvalidCredentialsException.hpp"
#include "../../../include/services/ClientCommunicationManager.h"
#include "../../../../Utils/Types.h"
#include "../../../../Utils/wniemiec/io/consolex/Consolex.hpp"

using namespace models::auth;
using namespace wniemiec::io::consolex;

//-------------------------------------------------------------------------
//      Constructor
//-------------------------------------------------------------------------
CLIAuthenticator::CLIAuthenticator(int argc, char* argv[])
{
    this->argc = argc;
    this->argv = argv;
}


//-------------------------------------------------------------------------
//      Methods
//-------------------------------------------------------------------------
void CLIAuthenticator::login()
{
    Consolex::write_info("Logging in...");

    do_login();

    if (!was_login_successful())
        throw exceptions::InvalidCredentialsException("Incorrect credentials");

    Consolex::write_info("Logged!");
}

void CLIAuthenticator::do_login()
{
    app_command loginCommand = models::command::CommandParser::parse(argc, argv);
    ClientCommunicationManager communicationModule;

    authResponseCode = communicationModule.establish_connection(
            loginCommand.arguments[0],
            loginCommand.arguments[1],
            loginCommand.arguments[2]
    );

    this->username = loginCommand.arguments[0];
}

bool CLIAuthenticator::was_login_successful()
{
    return (authResponseCode == 0);
}


//-------------------------------------------------------------------------
//      Getters
//-------------------------------------------------------------------------
std::string CLIAuthenticator::get_username()
{
    return username;
}
