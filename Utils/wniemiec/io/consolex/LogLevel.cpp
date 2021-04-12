#include "LogLevel.hpp"

using namespace wniemiec::io::consolex;

//-------------------------------------------------------------------------
//		Constructor
//-------------------------------------------------------------------------
LogLevel::LogLevel(bool error, bool warn, bool info, bool debug, std::string name)
{
    this->error = error;
    this->warn = warn;
    this->info = info;
    this->debug = debug;
    this->name = name;
}


//-------------------------------------------------------------------------
//		Factories
//-------------------------------------------------------------------------
LogLevel* LogLevel::OFF()
{
    return new LogLevel(false, false, false, false, "OFF");
}

LogLevel* LogLevel::ERROR()
{
    return new LogLevel(true, false, false, false, "ERROR");
}

LogLevel* LogLevel::WARNING()
{
    return new LogLevel(true, true, false, false, "WARN");
}

LogLevel* LogLevel::INFO()
{
    return new LogLevel(true, true, true, false, "INFO");
}

LogLevel* LogLevel::DEBUG()
{
    return new LogLevel(true, true, true, true, "DEBUG");
}


//-------------------------------------------------------------------------
//		Methods
//-------------------------------------------------------------------------
bool LogLevel::should_display_error()
{
    return error;
}

bool LogLevel::should_display_warning()
{
    return warn;
}

bool LogLevel::should_display_info()
{
    return info;
}

bool LogLevel::should_display_debug()
{
    return debug;
}


//-------------------------------------------------------------------------
//		Getters
//-------------------------------------------------------------------------
std::string LogLevel::get_name()
{
    return name;
}
