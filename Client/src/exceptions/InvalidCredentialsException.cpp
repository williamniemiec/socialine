#include <iostream>
#include "../../include/exceptions/InvalidCredentialsException.hpp"

using namespace exceptions;

//-------------------------------------------------------------------------
//      Constructor
//-------------------------------------------------------------------------
InvalidCredentialsException::InvalidCredentialsException(std::string message)
{
    this->message = message;
}


//-------------------------------------------------------------------------
//      Methods
//-------------------------------------------------------------------------
const char* InvalidCredentialsException::what() const throw()
{
    return this->message.c_str();
}
