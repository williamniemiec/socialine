//
// Created by Farias, Karine on 4/25/21.
//

#ifndef SOCIALINE_STRINGUTILS_H
#define SOCIALINE_STRINGUTILS_H

#include <iostream>
#include <vector>

namespace socialine::utils
{
    class StringUtils {

    public:
        /// Function that splits a string using the delimiter provided
        /// @param: str
        /// @param: sep
        /// @returns: a vector of strings containing substrings between the occurences of the separator
        static std::vector<std::string> split(std::string str, std::string sep);

        /// Function to convert all lower cases in a given string to upper case
        /// @param: str
        /// @returns: the given string with only upper case letters
        static std::string to_upper(std::string str);


        /// Function to remove whitespaces from both sides of a string
        /// @param: str
        /// @returns: the given string without spare whitespaces
        static std::string trim(const std::string& str);

    };

}

#endif //SOCIALINE_STRINGUTILS_H
