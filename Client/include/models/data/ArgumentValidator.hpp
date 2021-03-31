#pragma once

namespace models::data
{
    class ArgumentValidator
    {
    //-------------------------------------------------------------------------
    //      Constructor
    //-------------------------------------------------------------------------
    private:
        ArgumentValidator();


    //-------------------------------------------------------------------------
    //      Methods
    //-------------------------------------------------------------------------
    public:
        static bool validate_username(std::string username);
        static bool validate_ip(std::string ip);
        static bool validate_port(std::string port);
        static bool validate_command(std::string command);
    };
}
