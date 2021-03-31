#include "../../../include/models/auth/CLIAuthenticator.hpp"
#include "../../../include/models/command/CommandParser.hpp"
#include "../../../include/exceptions/InvalidCredentialsException.hpp"
#include "../../../include/services/ClientCommunicationManager.h"
#include "../../../../Utils/Types.h"

using namespace models::auth;

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
    std::cout << "Logging in..." << std::endl;

    doLogin();

    if (!wasLoginSuccessful())
        throw exceptions::InvalidCredentialsException("Incorrect credentials");

    std::cout << "Logged!" << std::endl;
}

void CLIAuthenticator::doLogin()
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

bool CLIAuthenticator::wasLoginSuccessful()
{
    return (authResponseCode == 0);
}


//-------------------------------------------------------------------------
//      Getters
//-------------------------------------------------------------------------
std::string CLIAuthenticator::getUsername()
{
    return username;
}