#pragma once
#include <iostream>

namespace wniemiec::io::consolex
{
    /// <summary>
    ///     Responsible for displaying log messages.
    /// </summary>
    class LogMessage
    {
    //-------------------------------------------------------------------------
    //		Attributes
    //-------------------------------------------------------------------------
    private:
        static const int PADDING_LEFT;
        std::string lastMessage;
        std::string lastMessageType;


    //-------------------------------------------------------------------------
    //		Constructor
    //-------------------------------------------------------------------------
    public:
        LogMessage();


    //-------------------------------------------------------------------------
    //		Methods
    //-------------------------------------------------------------------------
    public:
        /// <summary>
        ///     Display a log message.
        /// </summary>
        ///
        /// <param name="type">type Message type (error, warn, info...)</param>
        /// <param name="message">Message to be displayed</param>
        /// <param name="breakline">
        ///     Indicates whether there should be a line break after the log
        ///     message. Default is true.
        /// </param>
        void log(std::string type, std::string message, bool breakline);

        /// <summary>
        ///     Display a log message followed by a line break.
        /// </summary>
        ///
        /// <param name="type">type Message type (error, warn, info...)</param>
        /// <param name="message">Message to be displayed</param>
        void log(std::string type, std::string message);

        /// <summary>
        ///     Clears history.
        /// </summary>
        void clear();

    private:
        static std::string format_type(std::string type);


    //-------------------------------------------------------------------------
    //		Getters
    //-------------------------------------------------------------------------
    public:
        std::string get_last_message();
        std::string get_last_message_type();
    };
}
