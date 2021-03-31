#include <iostream>
#include "../../include/models/User.hpp"

using namespace models;

//-------------------------------------------------------------------------
//      Constructor
//-------------------------------------------------------------------------
User::User(std::string username)
{
    this->username = username;
}


//-------------------------------------------------------------------------
//      Getters
//-------------------------------------------------------------------------
std::string User::getUsername()
{
    return this->username;
}
