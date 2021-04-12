#pragma once
#include <iostream>

namespace wniemiec::io::consolex
{
    /// <summary>
    ///     Logging levels.
    /// </summary>
    class LogLevel
    {
    //-------------------------------------------------------------------------
    //		Attributes
    //-------------------------------------------------------------------------
    private:
        bool error;
        bool warn;
        bool info;
        bool debug;
        std::string name;


    //-------------------------------------------------------------------------
    //		Constructor
    //-------------------------------------------------------------------------
    private:
        LogLevel(bool error, bool warn, bool info, bool debug, std::string name);


    //-------------------------------------------------------------------------
    //		Factories
    //-------------------------------------------------------------------------
    public:
        /// <summary>
        ///     Disables all messages.
        /// </summary>
        static LogLevel* OFF();

        /// <summary>
        ///     Displays only error messages.
        /// </summary>
        static LogLevel* ERROR();

        /// <summary>
        ///     Displays error and warning messages.
        /// </summary>
        static LogLevel* WARNING();

        /// <summary>
        ///     Displays error, warning and info messages.
        /// </summary>
        static LogLevel* INFO();

        /// <summary>
        ///     Displays error, warning, info and debug messages.
        /// </summary>
        static LogLevel* DEBUG();


    //-------------------------------------------------------------------------
    //		Methods
    //-------------------------------------------------------------------------
    public:
        bool should_display_error();
        bool should_display_warning();
        bool should_display_info();
        bool should_display_debug();


    //-------------------------------------------------------------------------
    //		Getters
    //-------------------------------------------------------------------------
    public:
        std::string get_name();
    };
}
