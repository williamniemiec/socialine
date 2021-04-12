#include <iostream>
#include <string>
#include "InputConsolex.hpp"

using namespace wniemiec::io::consolex;

//-------------------------------------------------------------------------
//		Methods
//-------------------------------------------------------------------------
std::string InputConsolex::read_line()
{
    std::string input;

    std::getline(std::cin, input);

    return input;
}
