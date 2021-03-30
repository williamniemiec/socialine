//
// Created by Farias, Karine on 3/17/21.
//

#include "../include/Interface.h"

#include <iostream>
#include <vector>

app_command Interface::parse_command(int argc, char* argv[]) {

    app_command command;

    std::cout << "Hi! I am the Interface Module " << NEW_LINE;
    std::cout << "I am parsing the login command with following parameters: " << NEW_LINE;
    for(int i = 0; i <argc; i++)
    {
        std::cout << argv[i];
    }

    command.type = CMD_LOGIN;
    command.arguments.push_back("Profile");
    command.arguments.push_back("Server");
    command.arguments.push_back("Door");

    return command;
}

app_command Interface::parse_command(std::string input)
{
    app_command command;
    std::cout << "Hi! I am the Interface Module" << NEW_LINE;
    std::cout << "I will parse the following command: "<< input << NEW_LINE;

    std::string::size_type pos;
    pos= input.find(' ',0);
    std::string args = input.substr(pos+1);
    std::string cmd = input.substr(0,pos);

    command.type = get_command_type(cmd);
    command.arguments.push_back(args);

    return command;
}

void Interface::print_message(int message_code)
{
    std::cout << "Hi! I am the Interface Module" << NEW_LINE;
    switch(message_code)
    {
        case MESSAGE_SHUT_DOWN:
            std::cout << "Shutting down" << NEW_LINE;
            break;
        default:
            std::cout << "I am trying to print a message" << NEW_LINE;
    }

}

int Interface::get_command_type(std::string command_type_str) {
    if (command_type_str == "FOLLOW") {
        return CMD_FOLLOW;
    } else if (command_type_str == "SEND") {
        return CMD_TWEET;
    }

    return CMD_OTHERS;
}