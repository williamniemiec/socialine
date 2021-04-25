//
// Created by Farias, Karine on 4/25/21.
//

#include "StringUtils.h"

#include <iostream>
#include <vector>
#include <string.h>
#include <algorithm>

using namespace socialine::utils;

std::vector<std::string> StringUtils::split(std::string str, std::string sep)
{
    if (str.empty())
        return std::vector<std::string>();

    char* cstr=const_cast<char*>(str.c_str());
    char* current;
    std::vector<std::string> arr;

    current = strtok(cstr,sep.c_str());

    while(current != NULL)
    {
        arr.push_back(current);

        current = strtok(NULL,sep.c_str());
    }

    return arr;
}

std::string StringUtils::to_upper(std::string str)
{
    if (str.empty())
        return str;

    std::string upperString = str;

    std::transform(
            upperString.begin(),
            upperString.end(),
            upperString.begin(),
            ::toupper
    );

    return upperString;
}

std::string StringUtils::trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');

    if (std::string::npos == first)
        return str;

    size_t last = str.find_last_not_of(' ');

    return str.substr(first, (last - first + 1));
}

