#pragma once
#include <iostream>

namespace exceptions
{
    class InvalidCredentialsException : public std::exception
    {
    //-------------------------------------------------------------------------
    //      Attributes
    //-------------------------------------------------------------------------
    private:
        std::string message;


    //-------------------------------------------------------------------------
    //      Constructor
    //-------------------------------------------------------------------------
    public:
        InvalidCredentialsException(std::string message);


    //-------------------------------------------------------------------------
    //      Methods
    //-------------------------------------------------------------------------
    public:
        virtual const char* what() const noexcept override;
    };
}
