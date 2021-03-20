//
// Created by Farias, Karine on 3/17/21.
//

#include "include/ClientCommunicationManager.h"
#include "include/Interface.h"

#include <iostream>
#include <vector>


int main(int argc, char* argv[])
{
        Interface interfaceModule;
        ClientCommunicationManager communicationModule;
        app_command command_to_execute;
        std::string input;
        auto keep_running = true;
        int return_code;



        std::cout << "Hi, I am the main! I have just started!" << NEW_LINE;
        command_to_execute = interfaceModule.parse_command(argc, argv);

        //First command should always be "login"
        if( command_to_execute.type == CMD_LOGIN )
        {
            std::cout << "The command is: login"<< NEW_LINE;
            return_code = communicationModule.establish_connection(command_to_execute.arguments[1], command_to_execute.arguments[2], command_to_execute.arguments[3]);
        }
        else
        {
            std::cout << "My first command is not login" << NEW_LINE;
            interfaceModule.print_message(ERROR_START_WITH_LOGIN);
            keep_running = false;
        }

        std::cout << "Hi, I am back to the app. Waiting for new commands" << NEW_LINE;
        //After executing login keep waiting for new commands until user stops execution or an error occurs
        while( keep_running && std::getline(std::cin, input) )
        {
            command_to_execute = interfaceModule.parse_command(input);

            if (input.empty())
                continue;

            switch(command_to_execute.type)
            {
                case CMD_LOGIN:
                    std::cout << "I am already logged in" << NEW_LINE;
                    return_code = ERROR_SESSION_ALREADY_STARTED;
                    keep_running = false;
                    break;
                case CMD_FOLLOW:
                    std::cout << "The command is: follow" << NEW_LINE;
                    return_code = communicationModule.follow(command_to_execute.arguments[1]);
                    break;
                case CMD_OTHERS:
                    std::cout << "I don't recognise this command" << NEW_LINE;
                    return_code = ERROR_INVALID_COMMAND;
                    keep_running = false;
                    break;
            }

            interfaceModule.print_message(return_code);

        };

        interfaceModule.print_message(MESSAGE_SHUT_DOWN);

        return(0);

}