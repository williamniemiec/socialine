#include <iostream>
#include <vector>
#include <locale>
#include "../../../include/models/command/CommandBuilder.hpp"
#include "../../../include/models/command/CommandParser.hpp"
#include "../../../include/models/data/ArgumentValidator.hpp"
#include "../../../../Utils/StringUtils.hpp"

using namespace models::command;

//-------------------------------------------------------------------------
//      Constructor
//-------------------------------------------------------------------------
CommandParser::CommandParser()
{
}


//-------------------------------------------------------------------------
//      Methods
//-------------------------------------------------------------------------
app_command CommandParser::parse(int argc, char* argv[])
{
    validate_login(argc, argv);

    return CommandBuilder::build_login_command(argv[1], argv[2], argv[3]);
}

void CommandParser::validate_login(int argc, char* argv[])
{
    if (argc != 4)
        throw std::invalid_argument("Invalid args. Usage: <username> <server_ip> <port>");

    if (!models::data::ArgumentValidator::validate_username(argv[1]))
        throw std::invalid_argument("Invalid username. Expected: @[A-z0-9]+");

    if (!models::data::ArgumentValidator::validate_ip(argv[2]))
        throw std::invalid_argument("Invalid ip. Expected: [0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}");

    if (!models::data::ArgumentValidator::validate_port(argv[3]))
        throw std::invalid_argument("Invalid port. Expected: [0-9]{1,4}");
}

app_command CommandParser::parse(std::string command)
{
    validate_command(command);

    std::string::size_type idxFirstSpace = command.find(" ", 0);

    return parse(command.substr(0, idxFirstSpace), command.substr(idxFirstSpace+1));
}

app_command CommandParser::parse(std::string command, std::string arg)
{
    if (utils::StringUtils::to_upper(command) == "FOLLOW")
    {
        validate_username(arg);

        return CommandBuilder::build_follow_command(arg);
    }
    else if (utils::StringUtils::to_upper(command) == "SEND")
    {
        return CommandBuilder::build_send_command(arg);
    }
    else
    {
        throw std::invalid_argument("Command has no implementation: " + command);
    }
}

void CommandParser::validate_command(std::string command)
{
    if (!models::data::ArgumentValidator::validate_command(command))
        throw std::invalid_argument("Invalid command. Expected: <command> <param>");
}

void CommandParser::validate_username(std::string command)
{
    if (!models::data::ArgumentValidator::validate_username(command))
        throw std::invalid_argument("Invalid username. Expected: @[A-z0-9]+");
}