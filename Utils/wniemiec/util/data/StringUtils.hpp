#pragma once
#include <iostream>
#include <vector>

namespace wniemiec::util::data
{
    /// <summary>
    ///     Contains methods that perform string manipulation.
    /// </summary>
    class StringUtils
    {
    //-------------------------------------------------------------------------
    //      Constructor
    //-------------------------------------------------------------------------
    private:
        StringUtils();


    //-------------------------------------------------------------------------
    //      Methods
    //-------------------------------------------------------------------------
    public:
        /// <summary>
        ///     Returns a list of the words in the string, separated by the 
        ///     delimiter string.
        /// </summary>
        ///
        /// <param name="str">
        ///     String to be splited.
        /// </param>
        ///
        /// <param name="sep">
        ///     Character dividing the string into split groups.
        /// </param>
        ///
        /// <returns>Vector with split groups</returns>
        ///
        /// <exception cref="std::invalid_argument">
        ///     If string is null or if separator is null
        /// </exception>
        static std::vector<std::string> split(std::string str, std::string sep);

        /// <summary>
        ///     Returns a copy of the string in UPPER CASE.
        /// </summary>
        ///
        /// <param name="str">
        ///     Some string
        /// </param>
        ///
        /// <returns>String in upper case</returns>
        ///
        /// <exception cref="std::invalid_argument">
        ///     If string is null
        /// </exception>
        static std::string to_upper(std::string str);
		
		/// <summary>
        ///     Removes whitespaces from both sides of a string.
        /// </summary>
        ///
        /// <param name="str">
        ///     Some string
        /// </param>
        ///
        /// <returns>String with no whitespaces on both sides</returns>
		static std::string trim(const std::string& str);
    };
}
