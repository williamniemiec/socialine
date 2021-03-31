//
// Created by Farias, Karine on 3/31/21.
//

#include <iostream>
#include <vector>
#include <string.h>
#include <algorithm>
#include "StringUtils.hpp"

using namespace utils;

//-------------------------------------------------------------------------
//      Constructor
//-------------------------------------------------------------------------
StringUtils::StringUtils()
{
}


//-------------------------------------------------------------------------
//      Methods
//-------------------------------------------------------------------------
std::vector<std::string> StringUtils::split(std::string str, std::string sep)
{
    char* cstr=const_cast<char*>(str.c_str());
    char* current;
    std::vector<std::string> arr;

    current=strtok(cstr,sep.c_str());

    while(current!=NULL){
        arr.push_back(current);
        current=strtok(NULL,sep.c_str());
    }

    return arr;
}

std::string StringUtils::to_upper(std::string str)
{
    std::string upperString = str;

    std::transform(upperString.begin(), upperString.end(), upperString.begin(), ::toupper);

    return upperString;
}

