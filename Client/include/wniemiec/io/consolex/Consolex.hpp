#pragma once
#include <iostream>
#include <initializer_list>
#include <functional>
#include "OutputConsolex.hpp"
#include "InputConsolex.hpp"
#include "LogLevel.hpp"

namespace wniemiec::io::consolex
{
    /// <summary>
    ///     Facilitator of reading and writing on the console.
    /// </summary>
    class Consolex
    {
    //-------------------------------------------------------------------------
    //		Attributes
    //-------------------------------------------------------------------------
    private:
        static OutputConsolex* out;
        static InputConsolex* in;


    //-------------------------------------------------------------------------
    //		Constructor
    //-------------------------------------------------------------------------
    private:
        Consolex();


    //-------------------------------------------------------------------------
    //		Methods
    //-------------------------------------------------------------------------
    public:
        /// <summary>
        ///     Reads a line from console.
        /// </summary>
        ///
        /// <returns>Read line</returns>
        ///
        /// <exception cref=std::exception>
        ///     If it is not possible to read the console input.
        /// </exception>
        static std::string read_line();

        /// <summary>
        ///     Write a line on the console, putting a line break at the end.
        /// </summary>
        ///
        /// <param name="line">Line to be written</param>
        static void write_line(std::string line);

        /// <summary>
        ///     Write lines on the console, putting a line break at the end of
        ///     each line.
        /// </summary>
        ///
        /// <param name="lines">Lines to be written</param>
        static void write_lines(std::initializer_list<std::reference_wrapper<std::string>> lines);

        /// <summary>
        ///     Write lines on the console, putting a line break at the end of each
        ///     line.
        /// </summary>
        ///
        /// <param name="lines">Lines to be written</param>
        static void write_lines(std::list<std::string> lines);

        /// <summary>
        ///     Write a content on the console without a line break at the end.
        /// </summary>
        ///
        /// <param name="content">Content to be written</param>
        static void write(std::string content);

        /// <summary>
        ///     Write lines from a text file to the console.
        /// </summary>
        ///
        /// <param name="filepath">
        ///     File that will have its lines written on the console
        /// </param>
        ///
        /// <exception cref="std::exception">
        ///     If file cannot be read.
        /// </exception>
        static void write_file_lines(std::string filepath);

        /// <summary>
        ///     Write lines from a text file to the console. Besides, it shows the
        ///     line number of each line on the left.
        /// </summary>
        ///
        /// <param name="filepath">
        ///     File that will have its lines written on the console
        /// </param>
        ///
        /// <exception cref="std::exception">
        ///     If file cannot be read.
        /// </exception>
        static void write_file_lines_with_enumeration(std::string filepath);

        /// <summary>
        ///     Writes a division line.
        /// </summary>
        static void write_div();

        /// <summary>
        ///     Writes a division line.
        /// </summary>
        ///
        /// <param name="symbol">
        ///     Symbol that will compose the division.
        /// </param>
        static void write_div(std::string symbol);

        /// <summary>
        ///     Writes a title between two dividers.
        /// </summary>
        ///
        /// <param name="title">Header title</param>
        static void write_header(std::string title);

        /// <summary>
        ///     Writes a title between two dividers.
        /// </summary>
        ///
        /// <param name="title">Header title</param>
        /// <param name="symbol">Symbol that will compose the dividers</param>
        static void write_header(std::string title, std::string symbol);

        /// <summary>
        ///     Displays an error message. <br />
        ///     <b>Format:</b> <code>[ERROR] &lt;message&gt;</code>
        /// </summary>
        ///
        /// <param name="message">Message to be displayed</param>
        static void write_error(std::string message);

        /// <summary>
        ///     Displays an warning message. <br />
        ///     <b>Format:</b> <code>[WARN] &lt;message&gt;</code>
        /// </summary>
        ///
        /// <param name="message">Message to be displayed</param>
        static void write_warning(std::string message);

        /// <summary>
        ///     Displays an warning message. <br />
        ///     <b>Format:</b> <code>[INFO] &lt;message&gt;</code>
        /// </summary>
        ///
        /// <param name="message">Message to be displayed</param>
        static void write_info(std::string message);

        /// <summary>
        ///     Displays an warning message. <br />
        ///     <b>Format:</b> <code>[DEBUG] &lt;message&gt;</code>
        /// </summary>
        ///
        /// <param name="message">Message to be displayed</param>
        static void write_debug(std::string message);

        /// <summary>
        ///     Clears the history of messages sent to the console.
        /// </summary>
        static void clear_history();

        /// <summary>
        ///     Exports the history of messages sent to the console to a file.
        /// </summary>
        ///
        /// <param name="directoryPath">Output directory</param>
        static void dump_to(std::string directoryPath);


    //-------------------------------------------------------------------------
    //		Getters & Setters
    //-------------------------------------------------------------------------
    public:
        /// <summary>
        ///     Gets messages sent to the console.
        /// </summary>
        ///
        /// <returns>List of messages</returns>
        static std::list<std::string> get_history();

        /// <summary>
        ///     Defines the distance between messages and the log level tag.
        /// </summary>
        ///
        /// <param name="margin">Margin left</param>
        static void set_margin_left(int margin);

        /// <summary>
        ///     Sets log level. The level defines what type of message will be
        ///     displayed.
        /// </summary>
        ///
        /// <param name="level">New log level</param>
        static void set_logger_level(LogLevel* level);
    };
}
