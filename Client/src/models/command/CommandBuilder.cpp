#include <iostream>
#include "../../../include/models/command/CommandBuilder.hpp"

using namespace models::command;

//-------------------------------------------------------------------------
//      Constructor
//-------------------------------------------------------------------------
CommandBuilder::CommandBuilder()
{
}


//-------------------------------------------------------------------------
//      Methods
//-------------------------------------------------------------------------
app_command CommandBuilder::build_login_command(std::string username, std::string server, std::string port)
{
    app_command command;

    command.type = CMD_LOGIN;
    command.arguments.push_back(username);
    command.arguments.push_back(server);
    command.arguments.push_back(port);

    return command;
}

app_command CommandBuilder::build_follow_command(std::string followed)
{
    app_command commandContainer;

    commandContainer.type = CMD_FOLLOW;
    commandContainer.arguments.push_back(followed);

    return commandContainer;
}

app_command CommandBuilder::build_send_command(std::string message)
{
    app_command commandContainer;

    commandContainer.type = CMD_TWEET;
    commandContainer.arguments.push_back(message);

    return commandContainer;
}
