#pragma once
#include <iostream>
#include "IAuthenticator.hpp"

namespace models::auth
{
    class CLIAuthenticator : public IAuthenticator
    {
    //-------------------------------------------------------------------------
    //      Attributes
    //-------------------------------------------------------------------------
    private:
        std::string username;
        int argc;
        char** argv;
        int authResponseCode;


    //-------------------------------------------------------------------------
    //      Constructor
    //-------------------------------------------------------------------------
    public:
        CLIAuthenticator(int argc, char* argv[]);


    //-------------------------------------------------------------------------
    //      Methods
    //-------------------------------------------------------------------------
    public:
        virtual void login();
    private:
        void doLogin();
        bool wasLoginSuccessful();


    //-------------------------------------------------------------------------
    //      Getters
    //-------------------------------------------------------------------------
    public:
        virtual std::string getUsername();
    };
}
