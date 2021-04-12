//
// Created by Farias, Karine on 3/31/21.
//

#ifndef SOCIALINE_STRINGUTILS_H
#define SOCIALINE_STRINGUTILS_H

#pragma once

namespace utils
{
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
        static std::vector<std::string> split(std::string str,std::string sep);
        static std::string to_upper(std::string str);
		static std::string trim(const std::string& str);
    };
}

#endif
