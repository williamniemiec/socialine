#pragma once
#include <iostream>
#include "../../exceptions/InvalidCredentialsException.hpp"

namespace models::auth
{
    class IAuthenticator
    {
    //-------------------------------------------------------------------------
    //      Methods
    //-------------------------------------------------------------------------
    public:
        virtual void login() = 0;


    //-------------------------------------------------------------------------
    //      Methods
    //-------------------------------------------------------------------------
    public:
        virtual std::string get_username() = 0;
    };
}
