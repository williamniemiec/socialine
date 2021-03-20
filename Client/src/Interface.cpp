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

    command.type = CMD_FOLLOW;
    command.arguments.push_back("Profile");

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