#pragma once
#include <fstream>
#include "HistoryConsolex.hpp"
#include "LogLevel.hpp"
#include "LogMessage.hpp"

namespace wniemiec::io::consolex
{
    /// <summary>
    ///     Responsible for displaying log messages on the console.
    /// </summary>
    class LoggerConsolex
    {
    //-------------------------------------------------------------------------
    //		Attributes
    //-------------------------------------------------------------------------
    private:
        HistoryConsolex* history;
        LogLevel* level;
        LogMessage* logMessage;


    //-------------------------------------------------------------------------
    //		Constructor
    //-------------------------------------------------------------------------
    public:
        LoggerConsolex(HistoryConsolex* history);


    //-------------------------------------------------------------------------
    //		Methods
    //-------------------------------------------------------------------------
    public:
        void write_error(std::string message);
        void write_warning(std::string message);
        void write_info(std::string message);
        void write_debug(std::string message);
        template<typename T>
        void write_debug(T &source, std::string message);
        void dump_to(std::string output);

    private:
        template<typename T>
        std::string get_class_name(T &source);


    //-------------------------------------------------------------------------
    //		Setters
    //-------------------------------------------------------------------------
    public:
        /// <summary>
        ///     Sets log level. The level defines what type of message will be
        ///     displayed.
        /// </summary>
        ///
        /// <param name="level">New level</param>
        ///
        /// <exception cref="std::invalid_argument">
        ///      If level is null
        /// </exception>
        void set_level(LogLevel* level);
    };
}
