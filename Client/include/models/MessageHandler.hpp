#pragma once
#include <map>

namespace models
{
    class MessageHandler
    {
    //-------------------------------------------------------------------------
    //      Attributes
    //-------------------------------------------------------------------------
    private:
        static std::map<int, std::string> messageMapping;


    //-------------------------------------------------------------------------
    //      Constructor
    //-------------------------------------------------------------------------
    private:
        MessageHandler();


    //-------------------------------------------------------------------------
    //      Methods
    //-------------------------------------------------------------------------
    public:
        static std::string from_code(int code);
    private:
        static std::map<int, std::string> create_map();
    };
}
