#pragma once
#include <iostream>
#include "../../../../Utils/Types.h"

namespace models::command
{
    class CommandParser
    {
    //-------------------------------------------------------------------------
    //      Constructor
    //-------------------------------------------------------------------------
    private:
        CommandParser();


    //-------------------------------------------------------------------------
    //      Methods
    //-------------------------------------------------------------------------
    public:
        static app_command parse(int argc, char* argv[]);
        static app_command parse(std::string command);
    private:
        static app_command parse(std::string command, std::string arg);
        static void validate_login(int argc, char* argv[]);
        static void validate_command(std::string command);
        static void validate_username(std::string command);
    };
}
