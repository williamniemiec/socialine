#pragma once
#include <iostream>
#include "../../../../Utils/Types.h"

namespace models::command
{
    class CommandBuilder
    {
    //-------------------------------------------------------------------------
    //      Constructor
    //-------------------------------------------------------------------------
    private:
        CommandBuilder();


    //-------------------------------------------------------------------------
    //      Methods
    //-------------------------------------------------------------------------
    public:
        static app_command build_login_command(std::string username, std::string server, std::string port);
        static app_command build_follow_command(std::string followed);
        static app_command build_send_command(std::string message);
    };
}
